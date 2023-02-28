#ifndef IPOLL_HPP
# define IPOLL_HPP

# include <vector>

# include "Server.hpp"
# include "utils.hpp"
# include "Constants.hpp"
# include "Client.hpp"

class Server;

class IPoll {
	public:
		virtual ~IPoll() {};
		
		virtual bool init() = 0;
		virtual bool pollFd(int fd, int event) = 0;
		virtual bool deleteFd(int fd) = 0;
        virtual bool modFd(int fd, int event) = 0;
		virtual int clientConnect(Server &server) = 0;
		virtual int clientWrite(Client &client, Server &server) = 0;
		virtual int clientRead(Client &client, Server &server) = 0;
		virtual int polling(Server &server) = 0;

        virtual int pollOutEvent() const = 0;
        virtual int pollInEvent() const = 0;
};

#endif