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
		bool const pollFd(int fd, int event) const;
		bool const deleteFd(int fd) const;
		int const polling(Server &server) const;
		int const clientEvents() const;
        int const listenerEvents() const;

		EPoll &operator=(EPoll const &rhs);
};

#endif
