#include "server/nginx.hpp"

#include <utility>

nginx::nginx(std::string upstream_path) {
    this->_upstream_path = std::move(upstream_path);
}

void nginx::register_server(std::string ip_address, int port) {
    if (std::find(this->_servers_list.begin(),
                  this->_servers_list.end(),
                  ip_address) == this->_servers_list.end()) {
        this->_servers_list.push_back(ip_address);
        this->_application_ports[ip_address] = port;
    }
}

void nginx::cancel_registration() {
    this->check_servers_state();
}

int nginx::get_server_count() const {
    return this->_servers_list.size();
}

void nginx::check_servers_state() {
    std::vector<std::string> ip_to_add;
    std::vector<std::string> ip_to_remove;

    for (std::string &server : this->_servers_list) {
        if (std::find(this->_old_servers_list.begin(),
                      this->_old_servers_list.end(),
                      server) == this->_old_servers_list.end())
            ip_to_add.push_back(server);
    }
    for (std::string &server : this->_old_servers_list) {
        if (std::find(this->_servers_list.begin(),
                      this->_servers_list.end(),
                      server) == this->_servers_list.end())
            ip_to_remove.push_back(server);
    }
    if (ip_to_add.size() > 0 || ip_to_remove.size() > 0) {
        this->write_configuration_file();
        this->_old_servers_list = this->_servers_list;
    }
    this->_servers_list.clear();
}

void nginx::write_configuration_file() {
    std::ofstream file;

    file.open(this->_upstream_path + "/upstream.conf", std::ios::out | std::ios::trunc);
    for (std::string const &server : this->_servers_list)
        file << "server " << server << ':' << this->_application_ports[server] << ';' << std::endl;
    file << std::endl;
    file.close();

    system("nginx -s reload");
}