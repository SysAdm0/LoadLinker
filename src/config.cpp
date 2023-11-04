#include "config.hpp"

config::config() {
    _subnet = "";
    _mask = "";
}

std::string config::get_subnet() {
    return _subnet;
}

std::string config::get_mask() {
    return _mask;
}

void config::parse_config(std::string path) {
    std::ifstream ReadConfigFile(path);
    std::string delimiter = "=";
    std::string line;

    while (getline (ReadConfigFile, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.substr(0, line.find(delimiter)) == "subnet") {
            _subnet = line.substr(line.find(delimiter) + 1, line.length());
        } else if (line.substr(0, line.find(delimiter)) == "netmask") {
            _mask = line.substr(line.find(delimiter) + 1, line.length());
        }
    }
}