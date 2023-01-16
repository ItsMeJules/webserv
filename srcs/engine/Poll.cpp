#include "Poll.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Poll::Poll() {}
Poll::Poll(Poll const &Poll) { *this = Poll; }
Poll::~Poll() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

bool Poll::init() {
} // Not needed for classic polling

bool Poll::pollFd(int fd, int events) {
	struct pollfd event;
	event.fd = fd;
    event.events = events;
    bzero(&event.revents, sizeof(event.revents)); // Pour être safe et s'assurer que ça soit Clean.

    _pollfd.push_back(event); // to add to the vector
}

poll_it	Poll::addClient(int fd, int events, poll_it it) {
	struct pollfd event;
	event.fd = fd;
    event.events = events;
    bzero(&event.revents, sizeof(event.revents)); // Pour être safe et s'assurer que ça soit Clean.

    return (_pollfd.insert(it, event));
}

bool Poll::deleteFd(int fd) {
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	if (ret == -1)
		std::cerr << "failed to delete fd: " << fd << " from polling list!" << std::endl;
	else
		std::cerr << "sucessfully deleted fd: " << fd << " from polling list!" << std::endl;
	return ret != -1;
}


int Poll::polling(Server &server) {

	int readyFdAmount = poll(_pollfd.data(), _pollfd.size(), -1);
    if (readyFdAmount == -1) {
		std::cerr << "epoll_wait failed! error: " << strerror(errno) << std::endl;
		return -1;
	}

    for (poll_it it = _pollfd.begin(); it != _pollfd.end(); it++) {
        reset:
		if (it->revents != 0) {
			if (it->revents & (POLLERR | POLLHUP)) {
				if (close(it->fd) < 0) {
					std::cerr << "closer error on fd: " << std::endl;
					//TO DO 
				}
				it = _pollfd.erase(it);
				if (it == _pollfd.end())
					break;
				goto reset;
			}
			// Dans le cas du Serveur
			if (server.getSocket().getFd() == it->fd) {
				ClientSocket socket(server.getSocket().getFd());
            	if (!socket.setup())
                	return -3;

           		Client client(socket);
            	server.connect(client, it);
			} else {
				Client	&client = server.getClient(it->fd);
				if (it->revents & POLLIN) {
					server.receiveData(client);
				} else if (it->revents & POLLOUT) {
					std::cout << client.getRequestParser().getHttpRequest().build();
					HttpResponse	response("HTTP/1.1", 200, "OK");
					RegularBody		*body = new RegularBody();

					body->append("Hello World!");
					response.addHeader("Content-Type", "text/plain");
					response.addHeader("Content-Length", ws::itos(body->getSize()));
					response.setMessageBody(body);
					server.sendData(client, response);
					server.disconnect(client);
				}
			}
        }
        else
            continue;
    }

	return 1;
}


int Poll::clientEvents() {
	return POLLIN | POLLOUT;
} // Uniquement pour les clients

int Poll::listenerEvents() {
    return POLLIN;
} // Uniquement pour le server

// ############## GETTERS / SETTERS ##############
// ############## OPERATORS ##############

Poll &Poll::operator=(Poll const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
