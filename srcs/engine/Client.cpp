#include "Client.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Client::Client() {}
Client::Client(ClientSocket socket) : _socket(socket) {}
Client::Client(Client const &client) { *this = client; }
Client::~Client() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

bool Client::init() {
	return true;
}

// ############## GETTERS / SETTERS ##############

void Client::setSocket(ClientSocket socket) {
	_socket = socket;
}

ClientSocket &Client::getSocket() {
	return _socket;
}

RequestParser &Client::getRequestParser() {
	return _parser;
}

// ############## OPERATORS ##############

Client &Client::operator=(Client const &rhs) {
	if (this !=  &rhs) {
		_socket = rhs._socket;
		_parser = rhs._parser;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const Client &client) {
	os << "Client:{"
		<< "serverListeningFd: " << client._socket.getServerListeningFd() << ", " \
		<< "fd: " << client._socket.getFd() \
	<< "}";
	return os;
}
