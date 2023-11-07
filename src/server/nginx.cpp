#include "server/nginx.hpp"

void nginx::register_server(std::string ip_address) {
    if (std::find(this->_servers_list.begin(),
                  this->_servers_list.end(),
                  ip_address) == this->_servers_list.end()) {
        this->_servers_list.push_back(ip_address);
    }
}

void nginx::cancel_registration() {
    this->check_servers_state();
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
    std::cout << "Adding " << ip_to_add.size() << " servers" << std::endl;
    std::cout << "Removing " << ip_to_remove.size() << " servers" << std::endl;
    if (ip_to_add.size() > 0 || ip_to_remove.size() > 0) {
        this->write_configuration_file();
        this->_old_servers_list = this->_servers_list;
    }
    this->_servers_list.clear();
}

void nginx::write_configuration_file() {
    std::cout << "Writing configuration file" << std::endl;
    std::ofstream file;
    file.open("loadbalancer.conf", std::ios::out | std::ios::trunc);
    for (std::string &server : this->_servers_list)
        file << server << ':'<< 80 << ';' << std::endl;
    file << std::endl;
    file.close();
}