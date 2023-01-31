#include "DefaultBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

DefaultBody::DefaultBody(ADataDecoder *decoder) : AMessageBody(decoder) {}

DefaultBody::DefaultBody() {}
DefaultBody::DefaultBody(DefaultBody const &defaultBody) { *this = defaultBody; }

DefaultBody::~DefaultBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

int DefaultBody::parse(char *body, int &size) {
	int ret = 0;

	_decoder->addBuffer(body, size);
	do {
		ret = _decoder->decodeInto(_body);
	} while (ret == ws::DECODER_CALL_AGAIN || ret == ws::DECODER_PARSE_READY);
	
	return ret == ws::DECODER_STOP;
}

void DefaultBody::append(std::string str, int size) {
	for (int i = 0; i < size; i++)
		_body.push_back(str[i]);
}


// ############## GETTERS / SETTERS ##############

int DefaultBody::getBodySize() {
	return _body.size() - 1;
}

std::string DefaultBody::getBodyStr() {
	return std::string(_body.data(), _body.size() - 1);
}

// ############## OPERATORS ##############

DefaultBody &DefaultBody::operator=(DefaultBody const &rhs) {
	if (this != &rhs) {
		_body = rhs._body;
	}
	return *this;
}
