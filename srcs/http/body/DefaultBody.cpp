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
	return std::string(_body.data(), _body.size());
}

// ############## OPERATORS ##############

DefaultBody &DefaultBody::operator=(DefaultBody const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}
