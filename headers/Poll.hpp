#ifndef	POLL_HPP
# define POLL_HPP

# include <poll.h>

# include "IPoll.hpp"
# include "Server.hpp"
# include "Client.hpp"

class Poll : public IPoll {
	private:
		int	_pollFd;
	public:
		Poll();
		Poll(Poll const &Poll);
		~Poll();

		bool	init();
		bool	pollFd(int fd, int event);
		bool	deleteFd(int fd);
		int		polling(Server &server);
		int		clientEvents();
		int		listenerEvents();

		Poll	&operator=(Poll const &src);
};

#endif