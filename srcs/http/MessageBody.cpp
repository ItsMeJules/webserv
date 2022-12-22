#include "MessageBody.hpp"
#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

MessageBody::MessageBody() : _size(0) {}

MessageBody::MessageBody(std::string body) {
	_body << body;
	_size = body.length();
}

MessageBody::MessageBody(MessageBody const &messageBody) { *this = messageBody; }
MessageBody::~MessageBody() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

void MessageBody::append(std::string str) {
	std::cout << "appended: " << str << std::endl;
	_body << str;
	_size += str.size();
}

// ############## GETTERS / SETTERS ##############

std::string MessageBody::getBody() const {
	return _body.str();
}

std::string MessageBody::getSizeStr() const {
	return ws::itos(_size);
}

int MessageBody::getSize() const {
	return _size;
}

// ############## OPERATORS ##############
MessageBody &MessageBody::operator=(MessageBody const &rhs) {
	if (this != &rhs) {
		_body << rhs._body.str();
		_size = rhs._size;
	}
	return *this;
}