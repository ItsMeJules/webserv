#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>

# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <sys/socket.h>
# include <fcntl.h>

class IPoll;

class Socket {
	private:
	protected:
		int _fd;

		sockaddr_in _address;
	public:
		Socket();
		Socket(Socket const &socket);
		virtual ~Socket();

		virtual bool setup() = 0;
        virtual bool close(IPoll *poller) const;

		const int &getFd() const;
		const sockaddr_in &getAddress() const;

		Socket &operator=(Socket const &rhs);
};

#endif