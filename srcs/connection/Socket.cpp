# include "Socket.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Socket::Socket() {}
Socket::Socket(Socket const &socket) { *this = socket; }
Socket::~Socket() {}

// ############## PUBLIC ##############

bool Socket::close() const {
	bool ret = ::close(_fd) == 0;
	if (ret)
		std::cout << "[SOCKET] - successfully closed socket with fd: " << _fd << std::endl;
	else
		std::cout << "[SOCKET] - failed to close socket with fd: " << _fd << std::endl;
    return ret;
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