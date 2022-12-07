#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Connection.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <errno.h>

class Client : public Connection {
	private:
		int listeningFd_;

		Client();
	public:
		Client(int listeningFd);
		Client(Client const &client);
		~Client();

		bool generateFd();

		Client &operator=(Client const &rhs);
};

#endif