#pragma once

#include "agent/agent.hpp"
#include <fcntl.h>

enum ConfigType {
    SERVER_CONFIG = 1,
    AGENT_CONFIG = 2
};

class config {
    public:
        config();
        ~config() = default;

        void loadlinker_configure(ConfigType type, std::string path);
        std::map<std::string, std::string> get_server_config();
        std::map<std::string, std::string> get_agent_config();

    private:
        std::map<std::string, std::string> _loadlinker_server;
        std::map<std::string, std::string> _loadlinker_agent;
};

namespace {
    volatile std::sig_atomic_t gSignalStatus;
}