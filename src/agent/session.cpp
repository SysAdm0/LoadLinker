#include "agent/session.hpp"

session::session() {
    this->sock = 0;
    this->timeout.tv_sec = 1;
    this->timeout.tv_usec = 0;
}

void session::close_session() {
    close(this->sock);
}

void session::start_session(int wait_time) {
    send(this->sock, "", 0, 0);
    sleep(wait_time);
}

int session::connect_server(std::string ip_address, int port) {
    this->host_addr.sin_family = AF_INET;
    this->host_addr.sin_port = htons(port);
    this->host_addr.sin_addr.s_addr = inet_addr(ip_address.data());

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    inet_pton(AF_INET, ip_address.data(), &this->host_addr.sin_addr);
    setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&this->timeout, sizeof(this->timeout));
    setsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&this->timeout, sizeof(this->timeout));

    if (connect(this->sock, (struct sockaddr *)&this->host_addr, sizeof(this->host_addr))) {
        std::cout << "Connection failed" << std::endl;
        sleep(2);
        return 1;
    }
    return 0;
}