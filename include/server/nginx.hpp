#pragma once
#include "server/server.hpp"

class nginx {
    public:
        explicit nginx(std::string upstream_path);
        ~nginx() = default;

        void register_server(std::string ip_address, int port);
        void cancel_registration();
        int get_server_count() const;

    protected:
        void check_servers_state();
        void write_configuration_file();

    private:
        std::string _upstream_path;
        std::vector<std::string> _servers_list;
        std::vector<std::string> _old_servers_list;
        std::map<std::string, int> _application_ports;
};
