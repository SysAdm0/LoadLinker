#pragma once
#include "server/server.hpp"

class nginx {
    public:
        explicit nginx(std::string upstream_path);
        ~nginx() = default;

        void register_server(std::string ip_address, int port);
        void check_servers_state();

    protected:
        void write_configuration_file();
        int get_server_count() const;

    private:
        const int _check_time = 1;
        std::string _upstream_path;
        std::vector<std::vector<std::string>> _servers_list;
};
