#include "server/listener.hpp"

listener::listener() {
    _sock = 0;
    _timeout.tv_sec = 2;
    _timeout.tv_usec = 10;
}

void listener::run() {
    nginx nginx(this->_upstream_path);
    std::vector<std::thread> threads;
    std::cout << "LoadLinker Server is running..." << std::endl;

    while (1) {
        for (int i = 0; i < 100; i++) {
            threads.emplace_back([this, &nginx](){
                std::pair<std::string, int> host = accept_connection();
                if (!host.first.empty()) {
                    nginx.register_server(host.first, host.second);
                }
            });
        } for (auto& thread : threads)
            thread.join();
        threads.clear();
        nginx.cancel_registration();
    }
}

std::pair<std::string, int> listener::accept_connection() const {
    int client_sock = 0;
    char port[1024] = {0};
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    client_sock = accept(_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_sock < 0)
        return std::make_pair("", 0);
    recv(client_sock, port, 1024, 0);

    char *client_ip = inet_ntoa(client_addr.sin_addr);
    close(client_sock);
    try {
        return std::make_pair(client_ip, std::stoi(port));
    } catch (std::exception &e) {
        return std::make_pair("", 0);
    }
}

int listener::init_listener(std::map<std::string, std::string> config) {
    this->_host_addr.sin_family = AF_INET;
    this->_upstream_path = config["upstream_path"];
    this->_host_addr.sin_port = htons(std::stoi(config["listen_port"]));
    this->_host_addr.sin_addr.s_addr = inet_addr(config["bind_interface"].data());

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&this->_timeout, sizeof(this->_timeout));
    setsockopt(_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&this->_timeout, sizeof(this->_timeout));
    bind(this->_sock, (struct sockaddr *)&this->_host_addr, sizeof(this->_host_addr));
    listen(this->_sock, 5);
    return 0;
}