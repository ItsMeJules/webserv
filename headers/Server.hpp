#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"
# include "PollingManager.hpp"

class Server {
	private:
		Socket socket_;
		PollingManager pollingManager_;

		Server();
	public:
		Server(Socket socket);
		Server(Server const &server);
		~Server();

		bool initServer();
		bool monitorAllFd();
		bool listenOnSocket(int backlog);

		Socket getSocket() const;

		Server &operator=(Server const &rhs);
};

#endif