#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>

# include <sys/types.h>

# include "ServerSocket.hpp"
# include "IPoll.hpp"
# include "Poll.hpp"
# include "Client.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

# define BUFFER_SIZE 1000

class IPoll;

class Server {
	private:
		ServerSocket _socket;
		IPoll *_poller;

		std::map<int, Client> _clients;

		bool startListening(int backlog);
	public:
		Server();
		Server(ServerSocket &socket, IPoll *_poller);
		Server(Server const &server);
		~Server();

		void	receiveData(Client &client);
		void	sendData(Client &client, HttpResponse &response);
        bool 	connect(Client &client);
		poll_it connect(Client &client, poll_it it);
        bool 	disconnect(Client &client);

		Client &getClient(int fd);

		ServerSocket getSocket();
		IPoll *getPoller();

		Server &operator=(Server const &rhs);
};

#endif