#ifdef __linux__
# include "EPoll.hpp"
# include <stdio.h>
# include <iostream>
# include <string>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

EPoll::EPoll() {}
EPoll::EPoll(EPoll const &ePoll) { *this = ePoll; }
EPoll::~EPoll() {
    close(_epollFd);
}

// ############## PRIVATE ##############

std::string const EPoll::formatEvents(int const &events) const {
	std::string str;

	str += "\tEPOLLERR: " + ws::itos(events & EPOLLERR) + "\n";
	str += "\tEPOLLHUP: " + ws::itos(events & EPOLLHUP) + "\n";
	str += "\tEPOLLRDHUP: " + ws::itos(events & EPOLLRDHUP) + "\n";
	str += "\tEPOLLIN: " + ws::itos(events & EPOLLIN) + "\n";
	str += "\tEPOLLOUT: " + ws::itos(events & EPOLLOUT);
	return str;
}

// ############## PUBLIC ##############

bool EPoll::init() {
	_epollFd = epoll_create(10); //Nombre arbitraire (voir man page)
	return _epollFd != -1;
}

bool EPoll::pollFd(int fd, int events) {
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;

	int ret = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	return ret != -1;
}

bool EPoll::deleteFd(int fd) {
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	return ret != -1;
}

bool EPoll::modFd(int fd, int events) {
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;

    return epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event) != -1;
}

int EPoll::clientConnect(Server &server) {
	ClientSocket socket(server.getServerSocket().getFd());
	if (!socket.setup())
		return -3;

	Client client(socket);
	server.connect(client);
	return 1;
}

int EPoll::clientWrite(Client &client, Server &server) {
	HttpResponse response;
	DefaultBody *errorBody = new DefaultBody();

	if (!client.hasRequestFailed())
		response = client.getHttpRequest().execute(server.getServerInfo());
	else
		response.generateError(client.getRequestParser().getErrorCode(), server.getServerInfo(), *errorBody);

	server.sendData(client, response);

	if (client.getHttpRequest().headersContains("Connection", "close"))
		server.disconnect(client);
	// if there's no connection header we assume that the connection is keep-alive
	else {
		client.getRequestParser().clear();
		modFd(client.getSocket().getFd(), EPOLLIN);
	}

	delete errorBody;
	return 1;
}

int EPoll::clientRead(Client &client, Server &server) {
	if (!server.receiveData(client))
		server.disconnect(client);
	else if (client.hasRequestFailed())
		modFd(client.getSocket().getFd(), EPOLLOUT);
	else if (client.getRequestParser().isRequestParsed())
		modFd(client.getSocket().getFd(), EPOLLOUT);
	return 1;
}

int EPoll::polling(Server &server) {
	struct epoll_event events[ws::POLL_EVENTS_SIZE];

	int readyFdAmount = epoll_wait(_epollFd, events, ws::POLL_MAX_EVENTS, ws::POLL_WAIT_TIMEOUT);
	if (readyFdAmount == -1) {
		return -1;
	}

	for (int i = 0; i < readyFdAmount; i++) {
		if (events[i].events & EPOLLERR) {
			if (server.isConnected(events[i].data.fd))
				server.disconnect(server.getClient(events[i].data.fd));
			else
				deleteFd(events[i].data.fd);
			continue ;
		} else if (server.getServerSocket().getFd() == events[i].data.fd) { // Essai de connexion
			if (clientConnect(server) == -3)
				return -3;
        } else {
            Client &client = server.getClient(events[i].data.fd);

            if (events[i].events & EPOLLIN) {
				clientRead(client, server);
            } else if (events[i].events & EPOLLOUT) {
				clientWrite(client, server);
            } else if (events[i].events & EPOLLRDHUP)
                server.disconnect(client);
        }
 	}
	return 1;
}

int EPoll::pollOutEvent() const {
	return EPOLLOUT;
}

int EPoll::pollInEvent() const {
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
