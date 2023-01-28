#include "DefaultDataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

DefaultDataDecoder::DefaultDataDecoder(int contentLength) : _contentLength(contentLength), _sizeRead(0) {}

DefaultDataDecoder::DefaultDataDecoder() : _contentLength(-1), _sizeRead(0) {}
DefaultDataDecoder::DefaultDataDecoder(DefaultDataDecoder const &defaultDataDecoder) { *this = defaultDataDecoder; }

DefaultDataDecoder::~DefaultDataDecoder() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

int DefaultDataDecoder::decodeInto(char *buffer, int size, std::vector<char> &vec) {
	for (int i = 0; i < size; i++)
		vec.push_back(buffer[i]);
	_sizeRead += size;

	

	if (vec.size() >= _contentLength) {
		ws::log(ws::LOG_LVL_ALL, "[DefaultDataDecoder] -", "the whole request was read, " + ws::itos(_sizeRead) + " chars were parsed!");
		return 1;
	} else
		ws::log(ws::LOG_LVL_DEBUG, "[DefaultDataDecoder] -", "data stored in vector. Left to read " + ws::itos(_contentLength - _sizeRead));
	return 0;
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

DefaultDataDecoder &DefaultDataDecoder::operator=(DefaultDataDecoder const &rhs) {
	if (this != &rhs) {
		_contentLength = rhs._contentLength;
		_sizeRead = rhs._sizeRead;
	}
	return *this;
}
