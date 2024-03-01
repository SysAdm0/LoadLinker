#include "server/listener.hpp"

listener::listener() {
    _sock = 0;
    _timeout.tv_sec = 1;
    _timeout.tv_usec = 250000;
    _max_simultaneous_connections = 255;
}

void signal_handler(int signal) {
    gSignalStatus = signal;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void listener::run() {
    nginx nginx(this->_upstream_path);
    std::cout << "LoadLinker Server is running..." << std::endl;

    std::thread selectThread([this, &nginx]() {
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);
        std::signal(SIGQUIT, signal_handler);

        fd_set readfds;
        std::vector<int> agent_sock(this->_max_simultaneous_connections, 0);

        while (gSignalStatus != SIGINT && gSignalStatus != SIGTERM && gSignalStatus != SIGQUIT) {
            FD_ZERO(&readfds);
            FD_SET(_sock, &readfds);
            int max_sock = _sock;

            for (int conn = 0; conn < _max_simultaneous_connections; conn++) {
                if (agent_sock[conn] > 0)
                    FD_SET(agent_sock[conn], &readfds);
                if (agent_sock[conn] > max_sock)
                    max_sock = agent_sock[conn];
            }

            const int selected = select(max_sock + 1, &readfds, nullptr, nullptr, &_timeout);
            if (selected >= 0 && FD_ISSET(_sock, &readfds)) {
                int new_sock = accept_connection();
                for (int conn = 0; conn < _max_simultaneous_connections; conn++) {
                    if (new_sock >= 0 && agent_sock[conn] == 0) {
                        agent_sock[conn] = new_sock;
                        break;
                    }
                }
            }
            for (int conn = 0; conn < _max_simultaneous_connections; conn++) {
                if (selected >= 0 && FD_ISSET(agent_sock[conn], &readfds)) {
                    std::pair<std::string, int> host = receive_data(agent_sock[conn]);
                    if (!host.first.empty())
                        nginx.register_server(host.first, host.second);
                    close(agent_sock[conn]);
                    agent_sock[conn] = 0;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            nginx.check_servers_state();
        }
    });
    selectThread.join();
}

int listener::accept_connection() const {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    return accept(_sock, reinterpret_cast<struct sockaddr*>(&client_addr), reinterpret_cast<socklen_t*>(&client_addr_size));
}

std::pair<std::string, int> listener::receive_data(int sock) const {
    char port[1024] = {0};
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    recv(sock, port, 1024, 0);
    if (!is_number(port))
        return std::make_pair("", 0);
    getpeername(sock, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_size);
    char *client_ip = inet_ntoa(client_addr.sin_addr);
    return std::make_pair(client_ip, std::stoi(port));
}

int listener::init_listener(std::map<std::string, std::string> config) {
    int re_use = 1;
    int port = is_number(config["listen_port"]) ? std::stoi(config["listen_port"]) : 50000;
    this->_host_addr.sin_family = AF_INET;
    this->_host_addr.sin_port = htons(port);
    this->_upstream_path = config["upstream_path"];
    this->_host_addr.sin_addr.s_addr = inet_addr(config["bind_interface"].data());

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use));
    setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&_timeout), sizeof(_timeout));
    setsockopt(_sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&_timeout), sizeof(_timeout));
    bind(this->_sock, reinterpret_cast<struct sockaddr*>(&_host_addr), sizeof(_host_addr));
    listen(this->_sock, 5);
    return 0;
}