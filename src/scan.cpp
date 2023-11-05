#include "scan.hpp"

scan::scan() {
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
}

std::map<int, std::array<int, 2>> scan::ip_fragmentation(std::string ip_address, std::string netmask) {
    std::map<int, std::array<int, 2>> ip_fragments;
    std::map<std::string, std::string> mask = {
            std::make_pair("255", "1"),
            std::make_pair("254", "2"),
            std::make_pair("252", "4"),
            std::make_pair("248", "8"),
            std::make_pair("240", "16"),
            std::make_pair("224", "32"),
            std::make_pair("192", "64"),
            std::make_pair("128", "128"),
            std::make_pair("0", "255")
    };

    std::string delimiter = ".";
    for (int i = 0; i < 4; i++) {
        int primary = stoi(ip_address.substr(0, ip_address.find(delimiter)));
        int secondary = primary + (stoi(mask[netmask.substr(0, netmask.find(delimiter))]) - 1);
        secondary = secondary > 255 ? 255 : secondary;
        ip_fragments.insert(std::pair<int, std::array<int, 2>>(i, {primary, secondary}));
        ip_address.erase(0, ip_address.find(delimiter) + delimiter.length());
        netmask.erase(0, netmask.find(delimiter) + delimiter.length());
    }
    return ip_fragments;
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
    std::map<int, std::array<int, 2>> ip_fragments = ip_fragmentation(subnet, netmask);
    std::cout << "Scanning network... " << netmask << std::endl;

    for (int i = ip_fragments[0][0]; i <= ip_fragments[0][1]; i++)
        for (int j = ip_fragments[1][0]; j <= ip_fragments[1][1]; j++)
            for (int k = ip_fragments[2][0]; k <= ip_fragments[2][1]; k++)
                for (int l = ip_fragments[3][0]; l <= ip_fragments[3][1]; l++) {
                    std::string ip_address = std::to_string(i) + "." + std::to_string(j) + "." + std::to_string(k) + "." + std::to_string(l);
                    if (send_tcp_socket(ip_address) == 0)
                        std::cout << "Found a host at " << ip_address << "!" << std::endl;
                }
}