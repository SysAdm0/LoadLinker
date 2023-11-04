#include "scan.hpp"

scan::scan() {
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
}

int scan::send_tcp_socket(std::string ip_address) {
    fd_set fdset;

    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return 1;
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(this->port);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    if (inet_pton(AF_INET, ip_address.data(), &host_addr.sin_addr) <= 0)
        return 1;
    connect(sock, (struct sockaddr*)&host_addr, sizeof(host_addr));

    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);
    int status = select(sock + 1, NULL, &fdset, NULL, &timeout);
    close(sock);

    if (status == 1)
        return 0;
    return 1;
}

void scan::scan_network(std::string subnet, std::string netmask) {
    std::cout << "Scanning network... " << netmask << std::endl;
    if (send_tcp_socket(subnet) == 0)
        std::cout << "Found a host at " << subnet << "!" << std::endl;
    else
        std::cout << "No hosts found!" << std::endl;
}