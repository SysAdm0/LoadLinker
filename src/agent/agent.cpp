#include "agent/agent.hpp"

void signal_handler(int signal) {
    gSignalStatus = signal;
}

int main(const int argc, const char *argv[]) {
    config config;
    session session;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGQUIT, signal_handler);

    config.loadlinker_configure(AGENT_CONFIG, argc, argv);
    while (gSignalStatus != SIGINT && gSignalStatus != SIGTERM && gSignalStatus != SIGQUIT) {
        if (session.connect_server(config.get_agent_config()) == 0) {
            session::retry_after_timeout(false);
            session.start_session(1);
            session.close_session();
        } else {
            int waiting_connect = session::retry_after_timeout(true);
            std::cout << "Connection failed, retry after " << waiting_connect << " seconds." << std::endl;
            sleep(waiting_connect);
        }
    }
    return 0;
}