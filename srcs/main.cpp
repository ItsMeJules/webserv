# include "ServerSocket.hpp"
# include "Server.hpp"
# include "EPoll.hpp"

int main() {
	ServerSocket serverSocket;
	serverSocket.setup();
	
	IPoll *poller = new EPoll();
	Server server(serverSocket, poller);

	// std::cout << "" << std::endl;
	while (server.getPoller()->polling(server) > 0) {

	}
	delete poller;
}