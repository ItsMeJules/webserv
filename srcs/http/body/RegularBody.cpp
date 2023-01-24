#include "RegularBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RegularBody::RegularBody() : _size(0), _contentLength(-1) {}
RegularBody::RegularBody(int contentLength) : _size(0), _contentLength(contentLength) {}
RegularBody::RegularBody(RegularBody  const &regularBody ) { *this = regularBody; }
RegularBody::~RegularBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void RegularBody::append(std::string str, int size) {
	_body.append(str);
	_size += size;
}

int RegularBody::parse(std::string body, std::stringstream &inReceive, int const &byteCount) {
    append(body, byteCount);
	if (_size >= _contentLength) {
		ws::log(ws::LOG_LVL_ALL, "[REGULAR BODY] -", ws::itos(body.size()) + " chars from body was parsed");
		ws::log(ws::LOG_LVL_DEBUG, "[REGULAR BODY] -", "contents:\n" + body);
		return 1;
	} else
		ws::log(ws::LOG_LVL_DEBUG, "[REGULAR BODY] -", "data stored in stringstream. Left to read " + ws::itos(_contentLength - _size));
	return 0;
}

void RegularBody::truncBody(int pos, int npos) {
	_body.erase(pos, npos);
	_size -= npos;
}

// ############## GETTERS / SETTERS ##############

const std::string &RegularBody::getBody() const {
	return _body;
}

const int &RegularBody::getSize() const {
	return _size;
}

// ############## OPERATORS ##############

RegularBody  &RegularBody ::operator=(RegularBody  const &rhs) {
	if (this != &rhs) {
		_body = rhs._body;
		_size = rhs._size;
		_contentLength = rhs._contentLength;
	}
	return *this;
}
