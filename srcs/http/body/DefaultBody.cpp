#include "DefaultBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

DefaultBody::DefaultBody(ADataDecoder *decoder) : AMessageBody(decoder) {}

DefaultBody::DefaultBody() : AMessageBody() {}
DefaultBody::DefaultBody(DefaultBody const &defaultBody) : AMessageBody(*this) { *this = defaultBody; }

DefaultBody::~DefaultBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

int DefaultBody::parse(char *body, int &size) {
	int ret = 0;

	_decoder->addBuffer(body, size);
	do {
		ret = _decoder->decodeInto(_body);
	} while (ret == ws::DECODER_CALL_AGAIN || ret == ws::DECODER_PARSE_READY);
	
	return ret;
}

AMessageBody *DefaultBody::clone() {
	return new DefaultBody(*this);
}

void DefaultBody::append(std::string str, int size) {
	for (int i = 0; i < size; i++)
		_body.push_back(str[i]);
}

// ############## GETTERS / SETTERS ##############

int DefaultBody::getBodySize() {
	return _body.size();
}

std::string DefaultBody::getBodyStr() {
	if (_body.size() == 0)
		return "";
	return std::string(_body.data(), _body.size());
}

// ############## OPERATORS ##############

DefaultBody &DefaultBody::operator=(DefaultBody const &rhs) {
	if (this != &rhs) {
		_body = rhs._body;
	}
	AMessageBody::operator=(rhs);
	return *this;
}
