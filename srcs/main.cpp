#include <iostream>
#include <csignal>

#include <sys/stat.h>

#include "ServerSocket.hpp"
#include "Server.hpp"
#include "config_parser.hpp"
#include "utils.hpp"

#ifdef __linux__
# include "EPoll.hpp"
#else
# include "Poll.hpp"
#endif

#ifdef __linux__
	IPoll *Server::poller = new EPoll(); // EPOLL_LINUX
#else
	IPoll *Server::poller = new Poll(); // POLL_MAC
#endif

std::vector<Server*> Server::servers = std::vector<Server*>();

std::vector<Server *> vecserv;

namespace ws {

    bool setup_servers() {
        bool success = true;

        if (!ws::file_exists(ws::TMP_PATH)) {
            if (::mkdir(ws::TMP_PATH.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
                ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "failed to create the directory for temporary files!");
                return false;
            }
        }

        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
            Server *server = *it;
            std::string uploadPath = server->getServerInfo().getRootPath() + "/" + server->getServerInfo().getUploadPath();

            ws::checkConfiguration(server);

            if (!ws::file_exists(uploadPath)) {
                if (::mkdir(uploadPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
                    ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "failed to create the directory for uploaded files!");
                    return false;
                }
            }

            server->getServerSocket().setIp(server->getServerInfo().getIp());
            server->getServerSocket().setPort(server->getServerInfo().getPort());
            success = server->getServerSocket().setup() && server->setup();
        }
        return success;
    }

    void free_servers() {
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++)
            delete *it;
        HttpRequest::clearMethods();
    }

    void stop_webserv( int signum ) {
        ws::log(ws::LOG_LVL_INFO, "[MAIN] -", "Gracefully stopping webserv...");
        ws::free_servers();

        exit(signum);
    }

}

int main(int ac, char **av) {
    bool stop = false;
    signal(SIGINT, ws::stop_webserv);

    if (ac < 2) {
        ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "You must specify a configuration file!");
        return 1;
    }

    std::cout << "dans le main" << std::endl;

    try {
        ws::parseConfig(std::string(av[1]), Server::servers);
    } catch(const std::invalid_argument &e) {
        ws::free_servers();
        ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", std::string("Configuration file invalid: ") + e.what());
        ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "Stopping program...");
        return 1;
    }

    if (!ws::setup_servers()) {
        ws::free_servers();
        ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "Stopping program...");
        return 1;
    }

    while (!stop) {
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
            Server *server = *it;

            if (Server::poller->polling(*server) < 0) {
                ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "something went wrong with server: " + server->getServerInfo().getServerName());
                ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "Stopping program...");
                stop = true;
                break;
            }
        }
    }
    ws::free_servers();
    return 0;
}
