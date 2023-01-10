# include "Socket.hpp"
# include "IPoll.hpp" // the code doesn't work if i put this header in the hpp

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Socket::Socket() {}
Socket::Socket(Socket const &socket) { *this = socket; }
Socket::~Socket() {}

// ############## PUBLIC ##############

bool Socket::close(IPoll *poller) const {
    return poller->deleteFd(_fd) && ::close(_fd) == 0;
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