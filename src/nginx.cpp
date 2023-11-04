#include "config.hpp"

int main() {
    config conf;
    scan discovery;

    conf.parse_config("../network.conf");
    discovery.scan_network(conf.get_subnet(), conf.get_mask());
    return 0;
}