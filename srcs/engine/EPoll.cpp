#ifdef __linux__
# include "EPoll.hpp"

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

const bool EPoll::init() {
	ws::log(ws::LOG_LVL_INFO, "[EPOLL] -", "creating poll instance");
	_epollFd = epoll_create(10); //Nombre arbitraire (voir man page)
	if (_epollFd == -1)
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to create instance!", true);
	else                                                                                                                                                                
		ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "instance created with fd: " + ws::itos(_epollFd));
	return _epollFd != -1;
}

const bool EPoll::pollFd(int fd, int events) {
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;

	int ret = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	if (ret == -1)
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to add fd: " + ws::itos(fd) + " to polling list!", true);
	else
		ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "sucessfully added fd: " + ws::itos(fd) + " to polling list");
	ws::log(ws::LOG_LVL_DEBUG, "", "with events:\n " + formatEvents(events));
	return ret != -1;
}

const bool EPoll::deleteFd(int fd) {
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	if (ret == -1)
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to delete fd: " + ws::itos(fd) + " from polling list!", true);
	else
		ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "sucessfully deleted fd: " + ws::itos(fd) + " from polling list");
	return ret != -1;
}

const bool EPoll::modFd(int fd, int events) {
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;

    int ret = epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
    if (ret == -1)
        ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "failed to modify fd: " + ws::itos(fd) + "!", true);
    else
        ws::log(ws::LOG_LVL_SUCCESS, "[EPOLL] -", "successfully modified fd: " + ws::itos(fd));
	ws::log(ws::LOG_LVL_DEBUG, "", "with events:\n " + formatEvents(events));
    return ret != -1;
}

const int EPoll::polling(Server &server) {
	struct epoll_event events[ws::POLL_EVENTS_SIZE];

	int readyFdAmount = epoll_wait(_epollFd, events, ws::POLL_MAX_EVENTS, ws::POLL_WAIT_TIMEOUT);
	if (readyFdAmount == -1) {
		ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "waiting failed!", true);
		return -1;
	}

	for (int i = 0; i < readyFdAmount; i++) {
		if (events[i].events & EPOLLERR) {
			ws::log(ws::LOG_LVL_ERROR, "[EPOLL] -", "error on fd: " + ws::itos(events[i].data.fd) + "!");
			ws::log(ws::LOG_LVL_DEBUG, "", "with events:\n " + formatEvents(events[i].events));
			if (server.isConnected(events[i].data.fd))
				server.disconnect(server.getClient(events[i].data.fd));
			else
				deleteFd(events[i].data.fd);
			continue ;
		} else if (server.getSocket().getFd() == events[i].data.fd) { // Essai de connexion
			ws::log(ws::LOG_LVL_INFO, "[SERVER] - ", "connecting client...");
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