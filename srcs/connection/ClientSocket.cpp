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
	if (_fd == -1) {
		ws::log(ws::LOG_LVL_ERROR, "[CLIENT SOCKET] -", "accept failed!", true);
		return false;
	} else
		ws::log(ws::LOG_LVL_SUCCESS, "[CLIENT SOCKET] -", "client successfully connected with fd: " + ws::itos(_fd));
	return true;
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