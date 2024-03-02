#pragma once
#include "agent/agent.hpp"

#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class session {
    public:
        session();
        ~session() = default;

        int connect_server(std::map<std::string, std::string> config);
        static int retry_after_timeout(bool raise_timeout = true);
        void start_session(int wait_time);
        void close_session();

    private:
        struct sockaddr_in host_addr;
        struct timeval timeout;
        std::string app_port;
        int sock;
};
