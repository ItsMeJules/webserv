#ifndef ASOCKET_HPP
# define ASOCKET_HPP

# include <iostream>

# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <fcntl.h>

# include "utils.hpp"

class ASocket {
	private:
	protected:
		int _fd;

		sockaddr_in _address;
	public:
		ASocket();
		ASocket(ASocket const &socket);
		virtual ~ASocket();

		virtual bool setup() = 0;
        virtual bool close() const;

		const int &getFd() const;
		const sockaddr_in &getAddress() const;

		ASocket &operator=(ASocket const &rhs);
};

#endif