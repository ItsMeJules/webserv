#include "AMessageBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

AMessageBody::AMessageBody(ADataDecoder *decoder) : _decoder(decoder) {};

AMessageBody::AMessageBody() {}
AMessageBody::AMessageBody(AMessageBody const &messageBody) { *this = messageBody; }
AMessageBody::~AMessageBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

// ############## GETTERS / SETTERS ##############

ADataDecoder *AMessageBody::getDataDecoder() {
	return _decoder;
}

void AMessageBody::setDataDecoder(ADataDecoder *decoder) {
	_decoder = decoder;
}

// ############## OPERATORS ##############

AMessageBody &AMessageBody::operator=(AMessageBody const &rhs) {
	if (this != &rhs) {
		_decoder = rhs._decoder;
	}
	return *this;
}