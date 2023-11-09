#include "agent/agent.hpp"

int main() {
    config config;
    session session;

    config.loadlinker_configure(AGENT_CONFIG, "./loadlinker/network.conf");
    while (1) {
        if (session.connect_server(config.get_agent_config()) == 0) {
            session.start_session(1);
            session.close_session();
        }
    }
    return 0;
}