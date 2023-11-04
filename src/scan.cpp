#include "scan.hpp"

char *scan::craft_packet(std::string ip_address) {
    std::srand(static_cast<unsigned int>(time(nullptr)));
    std::memset(&IPheader, 0, sizeof(struct ip));
    std::memset(&TCPheader, 0, sizeof(struct tcphdr));

    TCPheader.th_seq = 0;
    TCPheader.th_off = 5;
    TCPheader.th_win = htons(1024);
    TCPheader.th_flags = TH_SYN;
    TCPheader.th_seq = htonl(23456);

    IPheader.ip_v = 4;
    IPheader.ip_hl = 5;
    IPheader.ip_tos = 0;
    IPheader.ip_off = 0;
    IPheader.ip_ttl = 16;
    IPheader.ip_p = IPPROTO_TCP;
    TCPheader.th_sport = htons(this->port);
    TCPheader.th_dport = htons(this->port);
    IPheader.ip_src.s_addr = inet_addr("172.0.0.11");
    IPheader.ip_dst.s_addr = inet_addr(ip_address.data());
    IPheader.ip_len = sizeof(struct ip) + sizeof(struct tcphdr);

    char *packet = new char[sizeof(struct iphdr) + sizeof(struct tcphdr)];
    std::memcpy(packet, &IPheader, sizeof(struct iphdr));
    std::memcpy(packet + sizeof(struct iphdr), &TCPheader, sizeof(struct tcphdr));

    return packet;
}

int scan::send_tcp_socket(std::string ip_address) {
    size_t packet_size = sizeof(struct iphdr) + sizeof(struct tcphdr);
    const int on = 1;
    int fd;

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(this->port);

    std::cout << "Sending TCP socket to " << ip_address << std::endl;
    if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
        return 1;
    if (inet_pton(AF_INET, ip_address.data(), &host_addr.sin_addr) <= 0)
        return 1;
    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
        return 1;

    char *packet = craft_packet(ip_address);
    if (sendto(fd, packet, packet_size, 0, (struct sockaddr*)&host_addr, sizeof(host_addr)) == -1) {
        delete[] packet;
        close(fd);
        return 1;
    }

// TODO: receive SYN/ACK
//
//    char buffer[2048];
//
//    ssize_t p_size = recv(fd, buffer, sizeof(buffer), 0);
//    if (p_size == -1) {
//        perror("recv");
//    }
//
//    struct tcphdr* receivedTcpHeader = (struct tcphdr*)(buffer + sizeof(struct iphdr));
//    if (receivedTcpHeader->th_flags == (TH_SYN | TH_ACK)) {
//        std::cout << "Received SYN/ACK from " << ip_address << std::endl;
//    }

    delete[] packet;
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