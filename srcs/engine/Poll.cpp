#include "Poll.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Poll::Poll() {}
Poll::Poll(Poll const &poll) { *this = poll; }
Poll::~Poll() {
    close(_pollFd);
}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

bool    Poll::init() {
    std::cout << "creating poll instance" << std::endl;
	std::cout << "epoll created with fd" << std::endl;
	return _pollFd != -1;
}

bool	Poll::pollFd(int fd, int events) {
	std::cout << "pollFd" << std::endl;
}

bool	Poll::deleteFd(int fd) {
	std::cout << "deleteFd succeed" << std::endl;
	return _pollFd != -1;
}

int		Poll::polling(Server &server) {
	struct pollfd events[EVENTS_SIZE];


	
	std::cout << "Poll Wait" << std::endl;
}

int		Poll::clientEvents() {
	return POLLIN | POLLOUT;
}

int		Poll::listenerEvents() {
	return POLLIN;
}

// ############## GETTERS / SETTERS ##############
// ############## OPERATORS ##############

Poll	&Poll::operator=(Poll const &src) {
	if (this != &src) {
		_pollFd = src._pollFd;
	}
	return *this;
}
