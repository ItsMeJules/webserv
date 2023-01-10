#ifndef IPOLL_HPP
# define IPOLL_HPP

# include "Server.hpp"
# include "utils.hpp"

# define MAX_EVENTS 10
# define EVENTS_SIZE 10
# define POLL_WAIT_TIMEOUT 60 * 1000

class Server;

class IPoll {
	public:
		virtual ~IPoll() {};
		virtual bool init() = 0;
		virtual const bool pollFd(int fd, int event) const = 0;
		virtual const bool deleteFd(int fd) const = 0;
		virtual const int polling(Server &server) const = 0;

        virtual const int clientEvents() const = 0;
        virtual const int listenerEvents() const = 0;
};

#endif