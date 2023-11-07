#pragma once

#include "agent/agent.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>

class session {
    public:
        session();
        ~session() = default;

        int connect_server(std::string ip_address, int port);
        void loop_session(int wait_time);
        void close_session();

    private:
        struct sockaddr_in host_addr;
        struct timeval timeout;
        int sock;
};