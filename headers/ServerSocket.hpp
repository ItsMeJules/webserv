#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include <iostream>
# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>

# include "ASocket.hpp"

class ServerSocket : public ASocket {
	private:
		int _domain;
		int _type;
		int _protocol;
		int _port;

        std::string _ip;

		bool generateFd();
		bool setReusable();
		bool setNonBlocking();
		bool bindTo();
	protected:
	public:
		ServerSocket();
		ServerSocket(int port);
        ServerSocket(std::string ip, int port);
		ServerSocket(ServerSocket const &socket);
		~ServerSocket();

		bool setup();

		const int &getPort() const;

        std::string const &getIp() const;

		void setPort(int port);
        void setIp(std::string ip);

		ServerSocket &operator=(ServerSocket const &rhs);
};

#endif