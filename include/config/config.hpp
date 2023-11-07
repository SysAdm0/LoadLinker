#pragma once

#include "agent/agent.hpp"
#include <fcntl.h>

#define SERVER_CONFIG 0;
#define AGENT_CONFIG 1;

class config {
    public:
        config();
        ~config() = default;

        void server_config(int argc, char **argv, int context);
        std::string get_ip_address();
        int get_bind_port();

    private:
        std::string _server;
        int _port;
};