#ifdef __linux__
# ifndef EPOLL_HPP
#  define EPOLL_HPP

#  include <vector>

#  include <sys/epoll.h>

#  include "IPoll.hpp"
#  include "Server.hpp"
#  include "Client.hpp"
# include "StatusCode.hpp"

class EPoll : public IPoll {
	private:
		int _epollFd;

		std::string const formatEvents(int const &events) const;
	public:
		EPoll();
		EPoll(EPoll const &ePoll);
		~EPoll();

		bool const init();
		bool const pollFd(int fd, int event);
		bool const deleteFd(int fd);
        bool const modFd(int fd, int event);
		int const polling(Server &server);
		
		int const clientEvents() const;
        int const listenerEvents() const;

		EPoll &operator=(EPoll const &rhs);
};

# endif
#endif