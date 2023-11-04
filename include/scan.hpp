#pragma once
#include "nginx.hpp"

class scan {
    public:
        scan() = default;
        ~scan() = default;

        static void scan_network(std::string subnet, std::string netmask);
};