#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <vector>

# include <sys/epoll.h>

# include "IPoll.hpp"
# include "Server.hpp"
# include "Client.hpp"

class EPoll : public IPoll {
	private:
		int _epollFd;
	public:
		EPoll();
		EPoll(EPoll const &ePoll);
		~EPoll();

		bool init();
		bool pollFd(int fd, int event);
		int polling(Server server);
		int readEvent();

		EPoll &operator=(EPoll const &rhs);
};

#endif