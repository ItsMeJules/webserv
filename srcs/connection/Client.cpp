#include "Client.hpp"

Client::Client() {}
Client::Client(int listeningFd) : listeningFd_(listeningFd) {}
Client::Client(Client const &client) {}

Client::~Client() {

}

Client &Client::operator=(Client const &rhs) {return *this;}

bool Client::generateFd() {
	socklen_t addrlen = sizeof(this->address_); // important sinon segv
	this->fd_ = accept(this->listeningFd_, (struct sockaddr*)&this->address_, (socklen_t*)&addrlen);
	if (this->fd_ == -1) {
		std::cerr << "error in accept: " << strerror(errno) << std::endl;
		return false;		
	} else
		std::cout << "Client sucessfully connected!" << std::endl;
	return true;
}