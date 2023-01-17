#include "Poll.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Poll::Poll() {}
Poll::Poll(Poll const &Poll) { *this = Poll; }
Poll::~Poll() {}

// ############## PRIVATE ##############
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
	std::cout << "sucessfully added fd: " << fd << " to polling list!" << std::endl;
	return true;
}

bool const Poll::deleteFd(int fd) {
	return true;
}

const bool Poll::modFd(int fd, int events) {
	// pas ouf je reitere dans les fd co. 
	// il faudrait que je change l'interface IPoll
    for (std::vector<pollfd_t>::iterator it = _pollFd.begin(); it != _pollFd.end(); it++) {
		if (it->fd == fd) {
			it->revents = events;
			std::cout << "successfully modified fd: " << fd << std::endl;
			return true;
		}
	}
	return false;
}

int const Poll::polling(Server &server) {
	int readyFdAmount = poll(_pollFd.data(), _pollFd.size(), POLL_WAIT_TIMEOUT);
    if (readyFdAmount == -1) {
		std::cerr << "poll failed! error: " << strerror(errno) << std::endl;
		return -1;
	}

	std::vector<pollfd_t> pollFdCopy = _pollFd;
    for (std::vector<pollfd_t>::iterator it = pollFdCopy.begin(); it != pollFdCopy.end(); it++) {
		if (it->revents == 0)
			continue ;
		if (it->revents & POLLERR) {
			std::cerr << "poll error on fd: " << it->fd << " with events " << it->revents << std::endl;
			if (server.isConnected(it->fd))
				server.disconnect(server.getClient(it->fd));
			else
				deleteFd(it->fd);
			continue ;
		} else if (server.getSocket().getFd() == it->fd) { // Dans le cas du Serveur
			ClientSocket socket(server.getSocket().getFd());
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
                } else {  // if there's no connection header we assume that the connection is keep-alive
                    client.getRequestParser().clear();
                    modFd(it->fd, POLLIN);
                }
            } else if (it->revents & POLLRDHUP)
                server.disconnect(client);
		}
    }
	return 1;
}


int const Poll::clientEvents() const {
	return POLLIN;
} // Uniquement pour les clients

int const Poll::listenerEvents() const {
    return POLLIN;
} // Uniquement pour le server

// ############## GETTERS / SETTERS ##############
// ############## OPERATORS ##############

Poll &Poll::operator=(Poll const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
