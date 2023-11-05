#pragma once
#include "nginx.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>

class scan {
    public:
        scan();
        ~scan() = default;

        void scan_network(std::string subnet, std::string netmask);

    protected:
        int send_tcp_socket(std::string ip_address);
        std::map<int, std::array<int, 2>> ip_fragmentation(std::string ip_address, std::string netmask);

    private:
        struct sockaddr_in host_addr;
        struct timeval timeout;
        int port = 80;
        int sock;
};