# include "ServerSocket.hpp"
# include "Server.hpp"
# include "EPoll.hpp"
# include "config_parser.hpp"

IPoll *Server::poller = new EPoll();
std::vector<Server*> Server::servers = std::vector<Server*>();

int main(int ac, char **av) {
    if (ac < 2) {
        ServerSocket serverSocket;
        serverSocket.setup();

        Server server(serverSocket);
        server.setup();

        while (Server::poller->polling(server) > 0) {
        }
        serverSocket.close();
        delete Server::poller;
    } else {
        ws::parse_config(std::string(av[1]), Server::servers);
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++)
            delete *it;
    }
}