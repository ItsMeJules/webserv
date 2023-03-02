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
                return false;
            }
        }

        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
            Server *server = *it;
            std::string uploadPath = server->getServerInfo().getRootPath() + "/" + server->getServerInfo().getUploadPath();

            ws::checkConfiguration(server);

            if (!ws::file_exists(uploadPath)) {
                if (::mkdir(uploadPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
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
        ws::free_servers();

        exit(signum);
    }

}

int main(int ac, char **av) {
    bool stop = false;
    signal(SIGINT, ws::stop_webserv);

    if (ac < 2) {
        return 1;
    }

    try {
        ws::parseConfig(std::string(av[1]), Server::servers);
    } catch(const std::invalid_argument &e) {
        ws::free_servers();
        return 1;
    }

    if (!ws::setup_servers()) {
        ws::free_servers();
        return 1;
    }

    while (!stop) {
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
            Server *server = *it;

            if (Server::poller->polling(*server) < 0) {
                stop = true;
                break;
            }
        }
    }
    ws::free_servers();
    return 0;
}
