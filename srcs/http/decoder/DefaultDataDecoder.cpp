#include "DefaultDataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

DefaultDataDecoder::DefaultDataDecoder(int contentLength) : _contentLength(contentLength), _sizeRead(0) {}

DefaultDataDecoder::DefaultDataDecoder() : _contentLength(-1), _sizeRead(0) {}
DefaultDataDecoder::DefaultDataDecoder(DefaultDataDecoder const &defaultDataDecoder) { *this = defaultDataDecoder; }

DefaultDataDecoder::~DefaultDataDecoder() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

int DefaultDataDecoder::decodeInto(std::vector<char> &vec) {
	vec.insert(vec.end(), _tmp.begin(), _tmp.end());
	_sizeRead += _tmp.size();

	if (_sizeRead == _contentLength) {
		if (!_tmp.empty()) {
			_tmp.clear();
			ws::log(ws::LOG_LVL_DEBUG, "[DefaultDataDecoder] -", "all data received.");
			return ws::DECODER_STOP;
		}

		_tmp.clear();
		ws::log(ws::LOG_LVL_ALL, "[DefaultDataDecoder] -", "the whole request was read, " + ws::itos(_sizeRead) + " chars were parsed!");
		return ws::DECODER_PARSE_READY;
	}

	_tmp.clear();
	ws::log(ws::LOG_LVL_DEBUG, "[DefaultDataDecoder] -", "data stored in vector. Left to read " + ws::itos(_contentLength - _sizeRead));
	return ws::DECODER_WAITING_FOR_RECV;
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
