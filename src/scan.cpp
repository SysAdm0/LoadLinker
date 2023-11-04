#include "scan.hpp"

int scan::send_tcp_socket(std::string ip_address) {
    struct sockaddr_in host_addr;
    int fd;

    std::cout << "Sending TCP socket to " << ip_address << std::endl;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return 1;

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, ip_address.data(), &host_addr.sin_addr) <= 0)
        return 1;
    if (connect(fd, (struct sockaddr*)&host_addr, sizeof(host_addr)) < 0)
        return 1;

    close(fd);
    return 0;
}

void scan::scan_network(std::string subnet, std::string netmask) {
    std::cout << "Scanning network... " << netmask << std::endl;
    if (send_tcp_socket(subnet) == 0)
        std::cout << "Found a host!" << std::endl;
    else
        std::cout << "No hosts found!" << std::endl;
}