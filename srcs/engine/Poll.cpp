#include "Poll.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Poll::Poll() {}
Poll::Poll(Poll const &Poll) { *this = Poll; }
Poll::~Poll() {}

// ############## PRIVATE ##############

std::string const Poll::formatEvents(int const &events) const {
	std::string str;

	str += "\tPOLLERR: " + ws::itos(events & POLLERR) + "\n";
	str += "\tPOLLERR: " + ws::itos(events & POLLERR) + "\n";
	str += "\tPOLLHUP: " + ws::itos(events & POLLHUP) + "\n";
	str += "\tPOLLIN: " + ws::itos(events & POLLIN) + "\n";
	str += "\tPOLLOUT: " + ws::itos(events & POLLOUT) + "\n";
	return str;
}

// ############## PUBLIC ##############

bool const Poll::init() {
	return true;
} // Not needed for classic polling

bool const Poll::pollFd(int fd, int events) {
	struct pollfd event;
	event.fd = fd;
    event.events = events;
    bzero(&event.revents, sizeof(event.revents)); // Pour être safe et s'assurer que ça soit Clean.

    _pollFd.push_back(event); // to add to the vector
	ws::log(ws::LOG_LVL_SUCCESS, "[POLL] -", "sucessfully added fd: " + ws::itos(fd) + " to polling vector");
	ws::log(ws::LOG_LVL_DEBUG, "[POLL] -", "with events:\n " + formatEvents(events));
	return true;
}

bool const Poll::deleteFd(int fd) {
	// pas ouf je reitere dans les fd co. 
	// il faudrait que je change l'interface IPoll
    for (std::vector<pollfd_t>::iterator it = _pollFd.begin(); it != _pollFd.end(); it++) {
		if (it->fd == fd) {
			ws::log(ws::LOG_LVL_SUCCESS, "[POLL] -", "successfully deleted fd: " + ws::itos(fd) + " from polling vector");
			return true;
		}
	}
	return true;
}

const bool Poll::modFd(int fd, int events) {
	// pas ouf je reitere dans les fd co. 
	// il faudrait que je change l'interface IPoll
    for (std::vector<pollfd_t>::iterator it = _pollFd.begin(); it != _pollFd.end(); it++) {
		if (it->fd == fd) {
			it->revents = events;
			ws::log(ws::LOG_LVL_SUCCESS, "[POLL] -", "successfully modified fd: " + ws::itos(fd));
			ws::log(ws::LOG_LVL_DEBUG, "[POLL] -", "with events:\n " + formatEvents(events));
			return true;
		}
	}
	return false;
}

int const Poll::polling(Server &server) {
	int readyFdAmount = poll(_pollFd.data(), _pollFd.size(), ws::POLL_WAIT_TIMEOUT);
    if (readyFdAmount == -1) {
		ws::log(ws::LOG_LVL_ERROR, "[POLL] -", "waiting failed!", true);
		return -1;
	}

	std::vector<pollfd_t> pollFdCopy = _pollFd;
    for (std::vector<pollfd_t>::iterator it = pollFdCopy.begin(); it != pollFdCopy.end(); it++) {
		if (it->revents == 0)
			continue ;
		if (it->revents & POLLERR) {
			ws::log(ws::LOG_LVL_ERROR, "[POLL] -", "error on fd: " + ws::itos(it->fd) + "!");
			ws::log(ws::LOG_LVL_DEBUG, "[POLL] -", "with events:\n " + formatEvents(it->revents));
			if (server.isConnected(it->fd))
				server.disconnect(server.getClient(it->fd));
			else
				deleteFd(it->fd);
			continue ;
		} else if (server.getServerSocket().getFd() == it->fd) { // Dans le cas du Serveur
			ClientSocket socket(server.getServerSocket().getFd());
			if (!socket.setup())
				return -3;

			Client client(socket);
			server.connect(client);
		} else { /* Dans le cas du Client */
			Client	&client = server.getClient(it->fd);
			if (it->revents & POLLIN) {
                if (!server.receiveData(client))
                    server.disconnect(client);
                else if (client.getRequestParser().isRequestParsed())
                    modFd(it->fd, POLLOUT);
            } else if (it->revents & POLLOUT) {
                HttpResponse response = client.getHttpRequest().execute(server.getServerInfo());
                DefaultBody *body = new DefaultBody();

				body->append("Hello World!", 13);
                response.addHeader("Content-Type", "text/plain");
                response.addHeader("Content-Length", ws::itos(body->getBody().size()));
                response.setMessageBody(body);
                server.sendData(client, response);
                if (client.getHttpRequest().headersContains("Connection", "close")) {
                    server.disconnect(client);
                } else {  // if there's no connection header we assume that the connection is keep-alive
                    client.getRequestParser().clear();
                    modFd(it->fd, pollInEvent());
                }
            } else if (it->revents & POLLHUP)
                server.disconnect(client);
		}
    }
	return 1;
}


int const Poll::pollOutEvent() const {
	return POLLOUT;
} // Uniquement pour les clients

int const Poll::pollInEvent() const {
    return POLLIN;
} // Uniquement pour le server

// ############## GETTERS / SETTERS ##############
// ############## OPERATORS ##############

Poll &Poll::operator=(Poll const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
