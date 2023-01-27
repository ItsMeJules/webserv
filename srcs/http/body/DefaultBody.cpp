#include "DefaultBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

DefaultBody::DefaultBody(ADataDecoder *decoder) : AMessageBody(decoder) {}

DefaultBody::DefaultBody() {}
DefaultBody::DefaultBody(DefaultBody const &defaultBody) { *this = defaultBody; }

DefaultBody::~DefaultBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

int DefaultBody::parse(char *body, int &size) {
	return _decoder->decodeInto(body, size, _body);
}

void DefaultBody::append(std::string str, int size) {
	for (int i = 0; i < size; i++)
		_body.push_back(str[i]);
}


// ############## GETTERS / SETTERS ##############

std::vector<char> &DefaultBody::getBody() {
	return _body;
}

std::string DefaultBody::getBodyStr() {
	std::cout << "size:" << _body.size() << std::endl;
	return std::string(_body.data(), _body.size() - 1);
}

// ############## OPERATORS ##############

DefaultBody &DefaultBody::operator=(DefaultBody const &rhs) {
	if (this != &rhs) {
		_body = rhs._body;
	}
	return *this;
}
