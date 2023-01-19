#ifndef IPOLL_HPP
# define IPOLL_HPP

# include <vector>

# include "Server.hpp"
# include "utils.hpp"
# include "Constants.hpp"

class Server;

class IPoll {
	public:
		virtual ~IPoll() {};
		
		virtual const bool init() = 0;
		virtual const bool pollFd(int fd, int event) = 0;
		virtual const bool deleteFd(int fd) = 0;
        virtual const bool modFd(int fd, int event) = 0;
		virtual const int polling(Server &server) = 0;

        virtual const int pollOutEvent() const = 0;
        virtual const int pollInEvent() const = 0;
};

#endif