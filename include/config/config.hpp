#pragma once

#include "agent/agent.hpp"

enum ConfigType {
    SERVER_CONFIG = 1,
    AGENT_CONFIG = 2
};

class config {
    public:
        config();
        ~config() = default;

        void check_loadlinker_config(ConfigType type);
        void loadlinker_configure(ConfigType type, int argc, const char *argv[]);
        static void display_help(ConfigType type, int argc, const char *argv[]);
        std::map<std::string, std::string> get_server_config();
        std::map<std::string, std::string> get_agent_config();

    private:
        std::map<std::string, std::string> _loadlinker_server;
        std::map<std::string, std::string> _loadlinker_agent;
};

namespace {
    volatile std::sig_atomic_t gSignalStatus;
}