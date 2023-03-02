#include "DefaultDataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

DefaultDataDecoder::DefaultDataDecoder(int contentLength) : _contentLength(contentLength), _sizeRead(0) {}

DefaultDataDecoder::DefaultDataDecoder() : _contentLength(-1), _sizeRead(0) {}
DefaultDataDecoder::DefaultDataDecoder(DefaultDataDecoder const &defaultDataDecoder) : ADataDecoder(*this) { *this = defaultDataDecoder; }

DefaultDataDecoder::~DefaultDataDecoder() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

int DefaultDataDecoder::decodeInto(std::vector<char> &vec) {
	if (_tmp.empty() && _sizeRead != _contentLength)
		return ws::DECODER_WAITING_FOR_RECV;
	vec.insert(vec.end(), _tmp.begin(), _tmp.end());
	_sizeRead += _tmp.size();

	if (_sizeRead == _contentLength) {
		if (!_tmp.empty()) {
			_tmp.clear();
			return ws::DECODER_PARSE_READY;
		} else {
			_sizeRead = 0;
			_contentLength = -1;
			return ws::DECODER_STOP;
		}
	} else if (_sizeRead > _contentLength)
		return ws::DECODER_DEFAULT_BODY_TOO_BIG;

	_tmp.clear();
	return ws::DECODER_PARSE_READY;
}

ADataDecoder *DefaultDataDecoder::clone() {
	return new DefaultDataDecoder(*this);
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
