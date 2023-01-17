#include "EPoll.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

EPoll::EPoll() {}
EPoll::EPoll(EPoll const &ePoll) { *this = ePoll; }
EPoll::~EPoll() {
    close(_epollFd);
}

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

const bool EPoll::pollFd(int fd, int events) const {
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;

	int ret = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	if (ret == -1)
		std::cerr << "failed to add fd: " << fd << " to polling list!" << std::endl;
	else
		std::cerr << "successfully added fd: " << fd << " to polling list!" << std::endl;
	return ret != -1;
}

const bool EPoll::deleteFd(int fd) const {
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	if (ret == -1)
		std::cerr << "failed to delete fd: " << fd << " from polling list!" << std::endl;
	else
		std::cerr << "successfully deleted fd: " << fd << " from polling list!" << std::endl;
	return ret != -1;
}

const bool EPoll::modFd(int fd, int events) const {
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;

    int ret = epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
    if (ret == -1)
        std::cerr << "failed to mod fd: " << fd << std::endl;
    else
        std::cerr << "successfully modified fd: " << fd << std::endl;
    return ret != -1;
}

const int EPoll::polling(Server &server) const {
	struct epoll_event events[EVENTS_SIZE];

	int readyFdAmount = epoll_wait(_epollFd, events, MAX_EVENTS, POLL_WAIT_TIMEOUT);
	if (readyFdAmount == -1) {
		std::cerr << "epoll_wait failed! error: " << strerror(errno) << std::endl;
		return -1;
	}

	for (int i = 0; i < readyFdAmount; i++) {
		if (events[i].events & EPOLLERR) {
			std::cerr << "epoll error on fd: " << events[i].data.fd << " with events " << events[i].events << std::endl;
			close(events[i].data.fd);
			return -2;
		}
        if (server.getSocket().getFd() == events[i].data.fd) { // Essai de connexion
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
                else
                    modFd(events[i].data.fd, EPOLLOUT);
            } else if (events[i].events & EPOLLOUT) {
                HttpResponse response("HTTP/1.1", 200, "OK");
                RegularBody *body = new RegularBody();
				body->append("Hello World!");
                response.addHeader("Content-Type", "text/plain");
                response.addHeader("Content-Length", ws::itos(body->getSize()));
                response.setMessageBody(body);
                server.sendData(client, response);
                if (client.getRequestParser().getHttpRequest().headersContains("Connection", "close")) {
                    server.disconnect(client);
                } else if (client.getRequestParser().getHttpRequest().headersContains("Connection", "keep-alive")) {
                    client.getRequestParser().clear();
                    modFd(events[i].data.fd, EPOLLIN);
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
