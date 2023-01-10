#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <vector>

# include <sys/epoll.h>

# include "IPoll.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "StatusCode.hpp"

class EPoll : public IPoll {
	private:
		int _epollFd;
	public:
		EPoll();
		EPoll(EPoll const &ePoll);
		~EPoll();

		bool init();
		bool pollFd(int fd, int event);
		bool deleteFd(int fd);
		int polling(Server &server);
		int clientEvents();
        int listenerEvents();

		EPoll &operator=(EPoll const &rhs);
};

#endif
