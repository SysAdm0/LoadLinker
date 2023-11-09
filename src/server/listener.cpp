#include "server/listener.hpp"

listener::listener() {
    _sock = 0;
    _timeout.tv_sec = 2;
    _timeout.tv_usec = 10;
}

void listener::run() {
    nginx nginx;
    std::vector<std::thread> threads;
    std::cout << "LoadLinker Server is running..." << std::endl;

    while (1) {
        for (int i = 0; i < 100; i++) {
            threads.emplace_back([this, &nginx](){
                std::string host = accept_connection();
                if (!host.empty()) {
                    nginx.register_server(host);
                }
            });
        } for (auto& thread : threads)
            thread.join();
        threads.clear();
        nginx.cancel_registration();
    }
}

std::string listener::accept_connection() const {
    int client_sock = 0;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    client_sock = accept(_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_sock < 0) {
        return "";
    }
    char *client_ip = inet_ntoa(client_addr.sin_addr);
    close(client_sock);
    return client_ip;
}

int listener::init_listener(int port) {
    this->_host_addr.sin_family = AF_INET;
    this->_host_addr.sin_port = htons(port);
    this->_host_addr.sin_addr.s_addr = INADDR_ANY;

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&this->_timeout, sizeof(this->_timeout));
    setsockopt(_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&this->_timeout, sizeof(this->_timeout));
    bind(this->_sock, (struct sockaddr *)&this->_host_addr, sizeof(this->_host_addr));
    listen(this->_sock, 5);
    return 0;
}