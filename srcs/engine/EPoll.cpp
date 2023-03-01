#ifdef __linux__
# include "EPoll.hpp"
# include <stdio.h>
# include <iostream>
# include <string>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

EPoll::EPoll() {}
EPoll::EPoll(EPoll const &ePoll) { *this = ePoll; }
EPoll::~EPoll() {
	ws::log(ws::LOG_LVL_INFO, "[EPOLL] -", "closing epoll instance");
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
	ws::log(ws::LOG_LVL_INFO, "[EPOLL] -", "creating poll instance");
	_epollFd = epoll_create(10); //Nombre arbitraire (voir man page)
	if (_epollFd == -1)
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to create instance!", true);
	else
		ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "instance created with fd: " + ws::itos(_epollFd));
	return _epollFd != -1;
}

bool EPoll::pollFd(int fd, int events) {
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;

	int ret = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	if (ret == -1)
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to add fd: " + ws::itos(fd) + " to polling list!");
	else
		ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "sucessfully added fd: " + ws::itos(fd) + " to polling list");
	ws::log(ws::LOG_LVL_DEBUG, "[EPOLL] -", "with events:\n " + formatEvents(events));
	return ret != -1;
}

bool EPoll::deleteFd(int fd) {
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	if (ret == -1)
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to delete fd: " + ws::itos(fd) + " from polling list!");
	else
		ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "sucessfully deleted fd: " + ws::itos(fd) + " from polling list");
	return ret != -1;
}

bool EPoll::modFd(int fd, int events) {
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;

    int ret = epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
    if (ret == -1) {
        ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to modify fd: " + ws::itos(fd) + "!");
	} else {
    	ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "successfully modified fd: " + ws::itos(fd));
	}

	ws::log(ws::LOG_LVL_DEBUG, "[EPOLL] -", "with events:\n " + formatEvents(events));
    return ret != -1;
}

int EPoll::clientConnect(Server &server) {
	ws::log(ws::LOG_LVL_INFO, "[SERVER] - ", "connecting client...");

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
	delete errorBody;

	if (client.getHttpRequest().headersContains("Connection", "close"))
		server.disconnect(client);
	// if there's no connection header we assume that the connection is keep-alive
	else {
		client.getRequestParser().clear();
		modFd(client.getSocket().getFd(), EPOLLIN);
	}
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
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "waiting failed!", true);
		return -1;
	}

	for (int i = 0; i < readyFdAmount; i++) {
		if (events[i].events & EPOLLERR) {
			ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "error on fd: " + ws::itos(events[i].data.fd) + "!");
			ws::log(ws::LOG_LVL_DEBUG, "[EPOLL] -", "with events:\n " + formatEvents(events[i].events));

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
