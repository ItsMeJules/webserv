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

		bool const init();
		bool const pollFd(int fd, int event);
		bool const deleteFd(int fd);
        bool const modFd(int fd, int event);
		int const polling(Server &server);

		int const pollOutEvent() const;
        int const pollInEvent() const;

		Poll &operator=(Poll const &rhs);
};

#endif