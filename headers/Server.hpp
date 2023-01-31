#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>

# include <sys/types.h>

# include "ServerSocket.hpp"
# include "IPoll.hpp"
# include "Client.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ServerInfo.hpp"
# include "Constants.hpp"

class IPoll;

class Server {
	private:
		ServerSocket _socket;
        ServerInfo _serverInfo;

		std::map<int, Client> _clients;

		bool startListening(int backlog);
    public:
        static IPoll *poller;
        static std::vector<Server*> servers;
	public:
		Server();
		Server(ServerSocket &socket);
		Server(Server const &server);
		~Server();

        bool setup();

		bool receiveData(Client &client);
		void sendData(Client &client, HttpResponse &response);
        bool connect(Client &client);
        bool disconnect(Client &client);
		bool isConnected(int const fd);

		Client &getClient(int const fd);

		ServerSocket &getServerSocket();
        ServerInfo &getServerInfo();

		void setServerSocket(ServerSocket serverSocket);

		Server &operator=(Server const &rhs);
};

#endif