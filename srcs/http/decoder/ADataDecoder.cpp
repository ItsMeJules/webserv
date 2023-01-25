#include "ADataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ADataDecoder::ADataDecoder() {
	_tmp.reserve(200); // arbitrary size
}

ADataDecoder::ADataDecoder(ADataDecoder const &aDataDecoder) { *this = aDataDecoder; }
ADataDecoder::~ADataDecoder() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

void ADataDecoder::fillTmp(char *buffer, size_t const &size) {
	if (_tmp.capacity() < size)
		_tmp.reserve(_tmp.size() + size);
		
	for (int i = 0; i < size; i++)
		_tmp.push_back(buffer[i]);
}

std::string ADataDecoder::bufferWithTmp(char *buffer, size_t const &size) {
	if (_tmp.empty())
		return std::string(buffer, size);
	return std::string(
				std::string(_tmp.data(), _tmp.size()) + 
				std::string(buffer, size));
}

// ############## PUBLIC ##############

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

ADataDecoder &ADataDecoder::operator=(ADataDecoder const &rhs) {
	if (this != &rhs) {
		_tmp = rhs._tmp;
	}
	return *this;
}
