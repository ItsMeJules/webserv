#include <iostream>
#include "Server.hpp"
#include "Client.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Server::Server() {
	
}

Server::Server(Socket socket) : socket_(socket) {
}

Server::Server(Server const &server) {

}

Server::~Server() {

}

// ############## PUBLIC ##############

bool Server::initServer() {
	if (!this->pollingManager_.createPollInstance())
		return false;
	if (!this->pollingManager_.pollFd(this->socket_.getFd(), EPOLLIN | EPOLLRDHUP | EPOLLERR))
		return false;
	return true;
}

bool Server::monitorAllFd() {
	while (true) {
		for (int i = 0; i < this->pollingManager_.getReadyFdAmount(); i++) {
			epoll_event pollEvent = this->pollingManager_.getEpollEvent(i);
			if (pollEvent.events & EPOLLIN) { // nested if pas ouf
				if (this->pollingManager_.isConnectionRequest(pollEvent, this->socket_.getFd())) {
					Client client(this->socket_.getFd());
					client.setupAddress();
					if (!client.generateFd())
						return false;
					this->pollingManager_.pollFd(client.getFd(), EPOLLIN | EPOLLRDHUP | EPOLLERR);
				} else { // data d'un client existant

				}
			}
		}
	}
}

bool Server::listenOnSocket(int backlog) {
	int ret = listen(this->socket_.getFd(), backlog);
	if (ret == -1)
		std::cerr << "Failed to to listen on fd: " << this->socket_.getFd() << strerror(errno) << std::endl;
	else
		std::cout << "Successfully listening on fd: " << this->socket_.getFd() << std::endl;
	return ret;
}

// ############## GETTERS / SETTERS ##############

Socket Server::getSocket() const {
	return socket_;
}


Server &Server::operator=(Server const &rhs) {
	return *this;
}
