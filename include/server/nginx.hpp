#pragma once
#include "server/server.hpp"

class nginx {
    public:
        nginx() = default;
        ~nginx() = default;

        void register_server(std::string ip_address);
        void cancel_registration();

    protected:
        void check_servers_state();
        void write_configuration_file();

    private:
        std::vector<std::string> _servers_list;
        std::vector<std::string> _old_servers_list;
};
