#include "RegularBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RegularBody::RegularBody() : _size(0) {}
RegularBody::RegularBody(RegularBody  const &regularBody ) { *this = regularBody; }
RegularBody::~RegularBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void RegularBody::append(std::string str) {
	append(str, str.size());
}

void RegularBody::append(std::string str, int size) {
	_body << str;
	_size += str.size();
}

int RegularBody::parse(std::string body, std::stringstream &inReceive) {
    append(body);
    return 1;
}

// ############## GETTERS / SETTERS ##############

std::string RegularBody::getBody() const {
	return _body.str();
}


std::string RegularBody::getMessage() const {
	return _msg.c_str();
}

int RegularBody::getSize() const {
	return _size;
}

// ############## OPERATORS ##############

RegularBody  &RegularBody ::operator=(RegularBody  const &rhs) {
	if (this != &rhs) {
		_body << rhs._body.str();
		_size = rhs._size;
	}
	return *this;
}
