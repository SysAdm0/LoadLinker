#include "server/nginx.hpp"

nginx::nginx(std::string upstream_path) {
    this->_upstream_path = std::move(upstream_path);
}

void nginx::register_server(std::string ip_address, int port) {
    std::vector<std::string> new_server;

    for (std::vector<std::string> &server : this->_servers_list) {
        if (server[0] == ip_address && server[1] == std::to_string(port)) {
            server[2] = std::to_string(std::time(nullptr));
            return;
        }
    }

    new_server.push_back(ip_address);
    new_server.push_back(std::to_string(port));
    new_server.push_back(std::to_string(std::time(nullptr)));
    this->_servers_list.push_back(new_server);
    this->write_configuration_file();
    new_server.clear();
}

int nginx::get_server_count() const {
    return this->_servers_list.size();
}

void nginx::check_servers_state() {
    int index = this->get_server_count();
    int server_count = index;

    for (int size = 0; size < server_count; size++) {
        if (std::time(nullptr) - std::stoi(this->_servers_list[size][2]) > this->_check_time) {
            this->_servers_list.erase(this->_servers_list.begin() + size);
            server_count--, size--;
        }
    }
    if (server_count != index)
        this->write_configuration_file();
}

void nginx::write_configuration_file() {
    std::ofstream file;

    file.open(this->_upstream_path + "/upstream.conf", std::ios::out | std::ios::trunc);
    for (std::vector<std::string> const &server : this->_servers_list)
        file << "server " << server[0] << ':' << server[1] << ';' << std::endl;
    file << std::endl;
    file.close();

    system("nginx -s reload");
}