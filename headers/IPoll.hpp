#ifndef IPOLL_HPP
# define IPOLL_HPP

# include "Server.hpp"

class Server;

class IPoll {
	public:
		virtual ~IPoll() {};
		virtual bool init() = 0;
		virtual bool pollFd(int fd, int event) = 0;
		virtual bool deleteFd(int fd) = 0;
		virtual int polling(Server &server) = 0;
		virtual int readEvent() = 0;
};

#endif