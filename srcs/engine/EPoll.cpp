#ifdef __linux__
# include "EPoll.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

EPoll::EPoll() {}
EPoll::EPoll(EPoll const &ePoll) { *this = ePoll; }
EPoll::~EPoll() {
    close(_epollFd);
}

// ############## PRIVATE ##############

void const EPoll::showEPollBits(int const &events) const {
	std::cout << "EPOLLERR: " << (events & EPOLLERR) << std::endl;
	std::cout << "EPOLLHUP: " << (events & EPOLLHUP) << std::endl;
	std::cout << "EPOLLRDHUP: " << (events & EPOLLRDHUP) << std::endl;
	std::cout << "EPOLLIN: " << (events & EPOLLIN) << std::endl;
	std::cout << "EPOLLOUT: " << (events & EPOLLOUT) << std::endl;
}

// ############## PUBLIC ##############

const bool EPoll::init() {
	std::cout << "[EPOLL] - creating poll instance" << std::endl;
	_epollFd = epoll_create(10); //Nombre arbitraire (voir man page)
	if (_epollFd == -1)
		std::cerr << "[EPOLL] - failed to create instance, error: " << strerror(errno) << std::endl;
	else                                                                                                                                                                
		std::cout << "[EPOLL] - instance created with fd: " << _epollFd << std::endl;
	return _epollFd != -1;
}

const bool EPoll::pollFd(int fd, int events) {
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;

	int ret = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	if (ret == -1)
		std::cerr << "[EPOLL] - failed to add fd: " << fd << " to polling list!" << std::endl;
	else
		std::cerr << "[EPOLL] - sucessfully added fd: " << fd << " to polling list!" << std::endl;
	return ret != -1;
}

const bool EPoll::deleteFd(int fd) {
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	if (ret == -1)
		std::cerr << "[EPOLL] - failed to delete fd: " << fd << " from polling list! " << strerror(errno) << std::endl;
	else
		std::cout << "[EPOLL] - sucessfully deleted fd: " << fd << " from polling list!" << std::endl;
	return ret != -1;
}

const bool EPoll::modFd(int fd, int events) {
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;

    int ret = epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
    if (ret == -1)
        std::cerr << "[EPOLL] - failed to modify fd: " << fd << std::endl;
    else
        std::cerr << "[EPOLL] - successfully modified fd: " << fd << std::endl;
    return ret != -1;
}

const int EPoll::polling(Server &server) {
	struct epoll_event events[ws::POLL_EVENTS_SIZE];

	int readyFdAmount = epoll_wait(_epollFd, events, ws::POLL_MAX_EVENTS, ws::POLL_WAIT_TIMEOUT);
	if (readyFdAmount == -1) {
		std::cerr << "[EPOLL] - waiting failed! error: " << strerror(errno) << std::endl;
		return -1;
	}

	for (int i = 0; i < readyFdAmount; i++) {
		if (events[i].events & EPOLLERR) {
			std::cerr << "[EPOLL] - error on fd: " << events[i].data.fd << " with events " << events[i].events << std::endl;
			showEPollBits(events[i].events);
			if (server.isConnected(events[i].data.fd))
				server.disconnect(server.getClient(events[i].data.fd));
			else
				deleteFd(events[i].data.fd);
			continue ;
		} else if (server.getSocket().getFd() == events[i].data.fd) { // Essai de connexion
            ClientSocket socket(server.getSocket().getFd());
            if (!socket.setup())
                return -3;

            Client client(socket);
            server.connect(client);
        } else {
            Client &client = server.getClient(events[i].data.fd);
            if (events[i].events & EPOLLIN) {
                if (!server.receiveData(client))
                    server.disconnect(client);
				else if (client.getRequestParser().isRequestParsed())
                    modFd(events[i].data.fd, EPOLLOUT);
            } else if (events[i].events & EPOLLOUT) {
				std::cout << client.getRequestParser().getHttpRequest().build() << std::endl;
                HttpResponse response("HTTP/1.1", 200, "OK");
                RegularBody *body = new RegularBody();

				body->append("Hello World!");
                response.addHeader("Content-Type", "text/plain");
                response.addHeader("Content-Length", ws::itos(body->getSize()));
                response.setMessageBody(body);
                server.sendData(client, response);
                if (client.getRequestParser().getHttpRequest().headersContains("Connection", "close")) {
                    server.disconnect(client);
                } else { // if there's no connection header we assume that the connection is keep-alive
                    client.getRequestParser().clear();
                    modFd(events[i].data.fd, clientEvents());
                }
            } else if (events[i].events & EPOLLRDHUP)
                server.disconnect(client);
        }
 	}
	return 1;
}


const int EPoll::clientEvents() const {
	return EPOLLIN;
}

const int EPoll::listenerEvents() const {
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

#endif