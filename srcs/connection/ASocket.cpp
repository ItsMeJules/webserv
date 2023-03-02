# include "ASocket.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ASocket::ASocket() {}
ASocket::ASocket(ASocket const &socket) { *this = socket; }
ASocket::~ASocket() {}

// ############## PUBLIC ##############

bool ASocket::close() const {
    return ::close(_fd) == 0;
}

// ############## GETTERS / SETTERS ##############
const int &ASocket::getFd() const {
	return _fd;
}

const sockaddr_in &ASocket::getAddress() const {
	return _address;
}

// ############## OPERATORS ##############

ASocket &ASocket::operator=(ASocket const &rhs) {
	if (this != &rhs) {
		_fd = rhs._fd;
		_address = rhs._address;
	}
	return *this;
}