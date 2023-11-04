#pragma once
#include "nginx.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class scan {
    public:
        scan() = default;
        ~scan() = default;

        void scan_network(std::string subnet, std::string netmask);

    protected:
        int send_tcp_socket(std::string ip_address);

    private:
        int port = 80;
};