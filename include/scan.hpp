#pragma once
#include "nginx.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

class scan {
    public:
        scan() = default;
        ~scan() = default;

        void scan_network(std::string subnet, std::string netmask);

    protected:
        int send_tcp_socket(std::string ip_address);
        char *craft_packet(std::string ip_address);

    private:
        int port = 113;
        struct ip IPheader;
        struct tcphdr TCPheader;
        struct sockaddr_in host_addr;
};