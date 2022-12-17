# include "MessageBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

MessageBody::MessageBody() {}
MessageBody::MessageBody(MessageBody const &messageBody) { *this = messageBody; }
MessageBody::~MessageBody() {}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

void MessageBody::append(std::string str) {
	_body << str;
}

// ############## GETTERS / SETTERS ##############

std::string MessageBody::getBody() const {
	return _body.str();
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