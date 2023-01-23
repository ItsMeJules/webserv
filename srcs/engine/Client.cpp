#include "Client.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Client::Client() : _requestFailed(false) {}
Client::Client(ClientSocket socket) : _socket(socket), _requestFailed(false) {}
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

void Client::setRequestFailed(bool requestFailed) {
	_requestFailed = requestFailed;
}


ClientSocket &Client::getSocket() {
	return _socket;
}

RequestParser &Client::getRequestParser() {
	return _parser;
}

HttpRequest &Client::getHttpRequest() {
	return _parser.getHttpRequest();
}

const bool &Client::hasRequestFailed() const {
	return _requestFailed;
}

// ############## OPERATORS ##############

Client &Client::operator=(Client const &rhs) {
	if (this !=  &rhs) {
		_socket = rhs._socket;
		_parser = rhs._parser;
		_requestFailed = rhs._requestFailed;
	}
	return *this;
}