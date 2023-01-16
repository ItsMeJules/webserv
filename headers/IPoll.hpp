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
		virtual bool pollFd(int fd, int event) = 0;
		virtual bool deleteFd(int fd) = 0;
		virtual int polling(Server &server) = 0;
		virtual poll_it	addClient(int fd, int events, poll_it it);

        virtual int clientEvents() = 0;
        virtual int listenerEvents() = 0;
};

#endif