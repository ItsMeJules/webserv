#include "EPoll.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

EPoll::EPoll() {}
EPoll::EPoll(EPoll const &ePoll) { *this = ePoll; }
EPoll::~EPoll() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

bool EPoll::init() {
	std::cout << "creating poll instance" << std::endl;
	_epollFd = epoll_create(10); //Nombre arbitraire (voir man page)
	if (_epollFd == -1) 
		std::cerr << "failed to create poll instance error: " << strerror(errno) << std::endl;
	else
		std::cout << "epoll created with fd: " << _epollFd << std::endl;
	return _epollFd != -1;
}

bool EPoll::pollFd(int fd, int events) {
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;

	int ret = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	if (ret == -1)
		std::cerr << "failed to add fd: " << fd << " to polling list!" << std::endl;
	else
		std::cerr << "sucessfully added fd: " << fd << " to polling list!" << std::endl;
	return ret != -1;
}

bool EPoll::deleteFd(int fd) {
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	if (ret == -1)
		std::cerr << "failed to delete fd: " << fd << " from polling list!" << std::endl;
	else
		std::cerr << "sucessfully deleted fd: " << fd << " from polling list!" << std::endl;
	return ret != -1;
}


int EPoll::polling(Server &server) {
	struct epoll_event events[10]; // Penser a mettre une constante

	int readyFdAmount = epoll_wait(_epollFd, events, 10, -1); // ici aussi
	if (readyFdAmount == -1) {
		std::cerr << "epoll_wait failed! error: " << strerror(errno) << std::endl;
		return -1;
	}
	
	for (int i = 0; i < readyFdAmount; i++) {
		if (!(events[i].events & EPOLLIN) || (events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
			std::cout << "epoll error on fd: " << events[i].data.fd << std::endl;
			close(events[i].data.fd);
			return -2;
		}
		if (server.getSocket().getFd() == events[i].data.fd) { // Essai de connexion
			ClientSocket socket(server.getSocket().getFd());
			if (!socket.setup())
				return -3;
				
			Client client(socket);
			pollFd(client.getSocket().getFd(), EPOLLIN);
			server.addClient(client);
		} else {
			Client client = server.getClient(events[i].data.fd);
			server.receiveData(client);
		}
 	}
	return 1;
}


int EPoll::readEvent() {
	return EPOLLIN;
}

// ############## GETTERS / SETTERS ##############
// ############## OPERATORS ##############

EPoll &EPoll::operator=(EPoll const &rhs) {
	if (this != &rhs) {
		_epollFd = rhs._epollFd;
	}
	return *this;
}
