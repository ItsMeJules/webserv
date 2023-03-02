#ifndef POLL_HPP
# define POLL_HPP

# include <vector>

# include <poll.h>

# include "IPoll.hpp"
# include "Server.hpp"
# include "Client.hpp"

class Poll : public IPoll {
	private:
		typedef struct pollfd pollfd_t;

        std::vector<pollfd_t> _pollFd;

		std::string const formatEvents(int const &events) const;
    public:
		Poll();
		Poll(Poll const &Poll);
		~Poll();

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

		Poll &operator=(Poll const &rhs);
};

#endif