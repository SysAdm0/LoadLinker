#include "config/config.hpp"

#include <vector>

config::config() {
    this->_loadlinker_server = {
            std::make_pair("upstream_path", ""),
            std::make_pair("bind_interface", ""),
            std::make_pair("listen_port", "")
    };
    this->_loadlinker_agent = {
            std::make_pair("server_address", ""),
            std::make_pair("server_port", ""),
            std::make_pair("application_port", "")
    };
}

std::map<std::string, std::string> config::get_server_config() {
    return this->_loadlinker_server;
}

std::map<std::string, std::string> config::get_agent_config() {
    return this->_loadlinker_agent;
}

void config::check_loadlinker_config(const ConfigType type) {
    std::map<std::string, std::string>::iterator it;
    switch (type) {
        case SERVER_CONFIG:
            for (it = this->_loadlinker_server.begin(); it != this->_loadlinker_server.end(); ++it) {
                if (it->second.empty()) {
                    std::cerr << "Error: " << it->first << " is not set" << std::endl;
                    exit(1);
                }
            } break;
        case AGENT_CONFIG:
            for (it = this->_loadlinker_agent.begin(); it != this->_loadlinker_agent.end(); ++it) {
                if (it->second.empty()) {
                    std::cerr << "Error: " << it->first << " is not set" << std::endl;
                    exit(1);
                }
            } break;
    }
}


void config::display_help(const ConfigType type, const int argc, const char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 || argc == 1) {
            switch (type) {
                case SERVER_CONFIG:
                    std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
                    std::cout << "Options:" << std::endl;
                    std::cout << "  -p, --port <port>           Listen on port" << std::endl;
                    std::cout << "  -i, --interface <interface> Bind to interface" << std::endl;
                    std::cout << "  -u, --upstream <path>       Nginx upstream path" << std::endl;
                    break;
                case AGENT_CONFIG:
                    std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
                    std::cout << "Options:" << std::endl;
                    std::cout << "  -p,  --port <port>             Server port" << std::endl;
                    std::cout << "  -ip, --ip_addr <ip>            Server IP address" << std::endl;
                    std::cout << "  -ap, --application_port <port> Nginx application port" << std::endl;
                    break;
            }
            exit(0);
        }
    }
}

void config::loadlinker_configure(ConfigType type, int argc, const char *argv[]) {
    display_help(type, argc, argv);
    for (int i = 1; i < argc; i++) {
        switch (type) {
            case SERVER_CONFIG:
                if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) {
                    this->_loadlinker_server.find("listen_port")->second = argv[i + 1];
                } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interface") == 0) {
                    this->_loadlinker_server.find("bind_interface")->second = argv[i + 1];
                } else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--upstream") == 0) {
                    this->_loadlinker_server.find("upstream_path")->second = argv[i + 1];
                } break;
            case AGENT_CONFIG:
                if (strcmp(argv[i], "-ip") == 0 || strcmp(argv[i], "--ip_addr") == 0) {
                    this->_loadlinker_agent.find("server_address")->second = argv[i + 1];
                } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) {
                    this->_loadlinker_agent.find("server_port")->second = argv[i + 1];
                } else if (strcmp(argv[i], "-ap") == 0 || strcmp(argv[i], "--application_port") == 0) {
                    this->_loadlinker_agent.find("application_port")->second = argv[i + 1];
                } break;
        }
    } check_loadlinker_config(type);
}