#include "ClientSocket.hpp"
 
// ############## CONSTRUCTORS / DESTRUCTORS ##############

ClientSocket::ClientSocket() : _serverListeningFd(-1) {}
ClientSocket::ClientSocket(int serverListeningFd) : _serverListeningFd(serverListeningFd) {}
ClientSocket::ClientSocket(ClientSocket const &clientSocket) : ASocket(*this) { *this = clientSocket; }
ClientSocket::~ClientSocket() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

bool ClientSocket::setup() {
	socklen_t addrlen = sizeof(_address); // important sinon segv
	_fd = accept(_serverListeningFd, (struct sockaddr*)&_address, (socklen_t*)&addrlen);
	
	return _fd != -1;
}

// ############## GETTERS / SETTERS ##############

const int &ClientSocket::getServerListeningFd() const {
	return _serverListeningFd;
}

// ############## OPERATORS ##############

ClientSocket &ClientSocket::operator=(ClientSocket const &rhs) { 
	if (this != &rhs) {
		_serverListeningFd = rhs._serverListeningFd;
		ASocket::operator=(rhs);
	}
	return *this;
}