#ifdef __linux__
# ifndef EPOLL_HPP
#  define EPOLL_HPP

#  include <vector>

#  include <sys/epoll.h>

#  include "IPoll.hpp"
#  include "Server.hpp"
#  include "Client.hpp"

class EPoll : public IPoll {
	private:
		int _epollFd;

		std::string const formatEvents(int const &events) const;
	public:
		EPoll();
		EPoll(EPoll const &ePoll);
		~EPoll();

		bool init();
		bool pollFd(int fd, int event);
		bool deleteFd(int fd);
        bool modFd(int fd, int event);
		int clientConnect(Server &server);
		int clientWrite(Client &client, Server &server);
		int clientRead(Client &client, Server &server);
		int polling(Server &server);

		int pollOutEvent() const;
        int pollInEvent() const;

		EPoll &operator=(EPoll const &rhs);
};

# endif
#endif