#include "config/config.hpp"

config::config() {
    _server = "";
    _port = 0;
}

std::string config::get_ip_address() {
    return _server;
}

int config::get_bind_port() {
    return _port;
}

void config::server_config(int argc, char **argv, int context) {
    int check_length = context == 0 ? 2 : 3;

    if (argc != check_length) {
        std::cout << "Error: bad command line arguments!" << std::endl;
        exit(1);
    } try {
        this->_server = context == 0 ? "" : argv[1];
        this->_port = context == 0 ?  std::stoi(argv[1]) : std::stoi(argv[2]);
    } catch (std::exception &e) {
        std::cout << "Error: bad command line arguments!" << std::endl;
        exit(1);
    }
}