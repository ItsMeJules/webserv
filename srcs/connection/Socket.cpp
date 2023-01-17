# include "Socket.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Socket::Socket() {}
Socket::Socket(Socket const &socket) { *this = socket; }
Socket::~Socket() {}

// ############## PUBLIC ##############

bool Socket::close() const {
    return ::close(_fd) == 0;
}

// ############## GETTERS / SETTERS ##############
const int &Socket::getFd() const {
	return _fd;
}

const sockaddr_in &Socket::getAddress() const {
	return _address;
}

// ############## OPERATORS ##############

Socket &Socket::operator=(Socket const &rhs) {
	if (this != &rhs) {
		_fd = rhs._fd;
		_address = rhs._address;
	}
	return *this;
}