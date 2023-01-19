#include "RegularBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RegularBody::RegularBody() : _size(0), _contentLength(-1) {}
RegularBody::RegularBody(int contentLength) : _size(0), _contentLength(contentLength) {}
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
	if (_size >= _contentLength) {
		ws::log(ws::LOG_LVL_ALL, "[REGULAR BODY] -", ws::itos(body.size()) + " chars from body was parsed");
		ws::log(ws::LOG_LVL_DEBUG, "[REGULAR BODY] -", "contents:\n" + body);
		return true;
	} else
		ws::log(ws::LOG_LVL_DEBUG, "[REGULAR BODY] -", "data stored in stringstream");
	return false;
}

bool RegularBody::excecute() {
	return true;
}

// ############## GETTERS / SETTERS ##############

const std::string RegularBody::getBody() const {
	return _body.str();
}

const int &RegularBody::getSize() const {
	return _size;
}

// ############## OPERATORS ##############

RegularBody  &RegularBody ::operator=(RegularBody  const &rhs) {
	if (this != &rhs) {
		_body << rhs._body.str();
		_size = rhs._size;
		_contentLength = rhs._contentLength;
	}
	return *this;
}
