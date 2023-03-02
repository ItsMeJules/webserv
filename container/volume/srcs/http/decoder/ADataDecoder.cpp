#include "ADataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ADataDecoder::ADataDecoder() {
	_tmp.reserve(200); // arbitrary size
}

ADataDecoder::ADataDecoder(ADataDecoder const &aDataDecoder) { *this = aDataDecoder; }
ADataDecoder::~ADataDecoder() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

std::string ADataDecoder::stringFromTmp(size_t const &size) {
	return std::string(_tmp.data(), size);
}

// ############## PUBLIC ##############

void ADataDecoder::addBuffer(const char *buffer, size_t const &size) {
	if (_tmp.capacity() < size)
		_tmp.reserve(_tmp.size() + size);
		
	for (size_t i = 0; i < size; i++)
		_tmp.push_back(buffer[i]);
}

// ############## GETTERS / SETTERS ##############

bool ADataDecoder::hasData() {
	return !_tmp.empty();
}

// ############## OPERATORS ##############

ADataDecoder &ADataDecoder::operator=(ADataDecoder const &rhs) {
	if (this != &rhs) {
		_tmp = rhs._tmp;
	}
	return *this;
}
