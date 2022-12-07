#include "Connection.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Connection::Connection() : domain_(AF_INET) {
	memset((char *)&this->address_, 0, sizeof(struct sockaddr_in));
}

Connection::Connection(int domain) : domain_(domain) {
	memset((char *)&this->address_, 0, sizeof(struct sockaddr_in));
}

Connection::Connection(Connection const &connection) {

}

Connection::~Connection() {

}

// ############## PUBLIC ##############

void Connection::setupAddress() {
	this->address_.sin_family = this->domain_;
	this->address_.sin_addr.s_addr = INADDR_ANY;
}

bool Connection::closeConnection() {
	return close(this->fd_) != -1;
}


// ############## GETTERS / SETTERS ##############

int Connection::getDomain() const {
	return this->domain_;
}

int Connection::getFd() const {
	return this->fd_;
}

sockaddr_in &Connection::getAddress() {
	return this->address_;
}

// ############## OPERATORS ##############

Connection &Connection::operator=(Connection const &rhs) {return *this;}
