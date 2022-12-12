#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>

# include "ServerSocket.hpp"
# include "IPoll.hpp"
# include "Client.hpp"

class IPoll;

class Server {
	private:
		ServerSocket _socket;
		IPoll *_poller;

		std::vector<Client> _clients;

		bool startListening(int backlog);
	public:
		Server();
		Server(ServerSocket &socket, IPoll *_poller);
		Server(Server const &server);
		~Server();

		void addClient(Client client);

		ServerSocket getSocket();
		IPoll *getPoller();

		Server &operator=(Server const &rhs);
};

#endif