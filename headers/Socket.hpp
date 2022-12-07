#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>

# include "Connection.hpp"

class Socket : public Connection {
	private:
	protected:
		int type_;
		int protocol_;
		int backlog_;

		bool binded_;
	public:
		Socket();
		Socket(int domain, int type, int protocol);
		Socket(Socket const &socket);
		~Socket();

		bool generateFd();
		bool bindTo(unsigned int port);
		bool setNonBlocking();
		bool setReusable();
		
		int getType() const;
		int getProtocol() const;
		int getBacklog() const;

		bool isBinded() const;

		Socket &operator=(Socket const &rhs);
};

#endif