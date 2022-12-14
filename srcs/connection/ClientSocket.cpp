#include "ClientSocket.hpp"
 
// ############## CONSTRUCTORS / DESTRUCTORS ##############

ClientSocket::ClientSocket() : _serverListeningFd(-1) {}
ClientSocket::ClientSocket(int serverListeningFd) : _serverListeningFd(serverListeningFd) {}
ClientSocket::ClientSocket(ClientSocket const &clientSocket) { *this = clientSocket; }
ClientSocket::~ClientSocket() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

bool ClientSocket::setup() {
	socklen_t addrlen = sizeof(_address); // important sinon segv
	_fd = accept(_serverListeningFd, (struct sockaddr*)&_address, (socklen_t*)&addrlen);
	if (_fd == -1) {
		std::cerr << "error in accept: " << strerror(errno) << std::endl;
		return false;		
	} else
		std::cout << "client sucessfully connected with fd: " << _fd << std::endl;
	return true;
}

// ############## GETTERS / SETTERS ##############

int ClientSocket::getServerListeningFd() const {
	return _serverListeningFd;
}

// ############## OPERATORS ##############

ClientSocket &ClientSocket::operator=(ClientSocket const &rhs) { 
	if (this != &rhs) {
		_serverListeningFd = rhs._serverListeningFd;
		Socket::operator=(rhs);
	}
	return *this;
}