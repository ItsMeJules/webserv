#include "Client.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Client::Client() : _socket(0) {}
Client::Client(ClientSocket socket) : _socket(socket) {}
Client::Client(Client const &client) { *this = client; }
Client::~Client() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

bool Client::init() {
	return true;
}

// ############## GETTERS / SETTERS ##############

ClientSocket &Client::getSocket() {
	return _socket;
}

void Client::setSocket(ClientSocket socket) {
	_socket = socket;
}

// ############## OPERATORS ##############

Client &Client::operator=(Client const &rhs) {
	if (this !=  &rhs) {
		_socket = rhs._socket;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const Client &client) {
	os << "Client:{"
				<< "serverListeningFd: " << client._socket.getServerListeningFd() \
				<< ", fd: " << client._socket.getFd() \
				<< "}";
	return os;
}
