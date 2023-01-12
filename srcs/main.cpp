# include "ServerSocket.hpp"
# include "Server.hpp"
# include "EPoll.hpp"
# include "config_parser.hpp"

int main(int ac, char **av) {
    if (ac == 1) {
        ServerSocket serverSocket;
        serverSocket.setup();

        IPoll *poller = new EPoll();
        Server server(serverSocket, poller);

        while (poller->polling(server) > 0) {
        }
        serverSocket.close(poller);
        delete poller;
    } else {
        ws::parse_config(std::string(av[1]), Server::servers);
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++)
            delete *it;
    }
}