#pragma once
#include "nginx.hpp"

class config {
    public:
        config();
        ~config() = default;

        void parse_config(std::string path);
        std::string get_subnet();
        std::string get_mask();

    private:
        std::string _subnet;
        std::string _mask;
};