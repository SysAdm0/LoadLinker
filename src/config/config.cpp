#include "config/config.hpp"

config::config() {
    // server default config
    this->_loadlinker_server = {
            std::make_pair("upstream_path", "/etc/nginx/loadlinker/"),
            std::make_pair("bind_interface", "0.0.0.0"),
            std::make_pair("listen_port", "50000")
    };
//    // agent default config
    this->_loadlinker_agent = {
            std::make_pair("server_address", ""),
            std::make_pair("server_port", "50000"),
            std::make_pair("application_port", "8080")
    };
}

std::map<std::string, std::string> config::get_server_config() {
    return this->_loadlinker_server;
}

std::map<std::string, std::string> config::get_agent_config() {
    return this->_loadlinker_agent;
}

void config::loadlinker_configure(ConfigType type, std::string path) {
    std::ifstream ReadConfigFile(path);
    std::string delimiter = "=";
    std::string line;

    if (!ReadConfigFile.is_open())
        std::cout << "Unable to find network.conf file\n --> Using default configuration" << std::endl;
    while (getline (ReadConfigFile, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        std::string key = line.substr(0, line.find(delimiter));
        switch (type) {
            case SERVER_CONFIG:
                if (this->_loadlinker_server.find(key) != this->_loadlinker_server.end()) {
                    this->_loadlinker_server[key] = line.substr(line.find(delimiter) + 1, line.length());
                } break;
            case AGENT_CONFIG:
                if (this->_loadlinker_agent.find(key) != this->_loadlinker_agent.end()) {
                    this->_loadlinker_agent[key] = line.substr(line.find(delimiter) + 1, line.length());
                } break;
        }
    }
}