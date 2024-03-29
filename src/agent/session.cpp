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
    send(this->sock, this->app_port.data(), this->app_port.length(), 0);
    sleep(wait_time);
}

int session::retry_after_timeout(bool raise_timeout) {
    static int retry = 1;

    if (raise_timeout)
        retry = retry < 32 ? retry * 2 : 32;
    else
        retry = 1;
    return retry;
}

int session::connect_server(std::map<std::string, std::string> config) {
    this->host_addr.sin_family = AF_INET;
    this->app_port = config["application_port"];
    this->host_addr.sin_port = htons(std::stoi(config["server_port"]));
    this->host_addr.sin_addr.s_addr = inet_addr(config["server_address"].data());

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    inet_pton(AF_INET, config["server_address"].data(), &this->host_addr.sin_addr);
    setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&this->timeout), sizeof(this->timeout));
    setsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&this->timeout), sizeof(this->timeout));

    if (connect(this->sock, reinterpret_cast<struct sockaddr*>(&this->host_addr), sizeof(this->host_addr)))
        return 1;
    return 0;
}