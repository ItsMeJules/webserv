#include "ChunkedDataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ChunkedDataDecoder::ChunkedDataDecoder() {
	clearActualChunk();
}

ChunkedDataDecoder::ChunkedDataDecoder(ChunkedDataDecoder const &chunkedDataDecoder) : ADataDecoder(*this) { *this = chunkedDataDecoder; }
ChunkedDataDecoder::~ChunkedDataDecoder() {}

// ############## PRIVATE ##############

void ChunkedDataDecoder::clearActualChunk() {
	_actualChunk.size = -1;
	_actualChunk.content.clear();
	_sizeRead = 0;
}

int ChunkedDataDecoder::readChunkSize(size_t const &endChunkSizePos) {
	std::string sizeStr = ADataDecoder::stringFromTmp(endChunkSizePos);
	if (!ws::string_in_range(ws::HEX_VALUES, sizeStr)) {
		ws::log(ws::LOG_LVL_ERROR, "[ChunkedDataDecoder] -", "error while reading chunk size. \"" + sizeStr + "\" isn't a valid hex value!");
		return ws::DECODER_CHUNKED_NOT_HEX;
	}

	_actualChunk.size = ws::hextoi(sizeStr);
	if (_actualChunk.size != 0) {
		ws::log(ws::LOG_LVL_ALL, "[ChunkedDataDecoder] -", "a chunk of " + ws::itos(_actualChunk.size) + " chars is about to be parsed");
		_actualChunk.content.reserve(_actualChunk.size);
		_tmp.erase(_tmp.begin(), _tmp.begin() + endChunkSizePos + 2); // not very efficient.
		return ws::DECODER_CALL_AGAIN;
	} else {
		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "all chunks were received & parsed!");
		return ws::DECODER_STOP; // all chunks were read
	}
}

bool ChunkedDataDecoder::checkChunkSize(int const &bufSize, int const &read) {
	if (read != bufSize && read + 2 <= bufSize) {
		ws::log(ws::LOG_LVL_ERROR, "[ChunkedDataDecoder] -",
			"error while reading a chunk, the chunk size is bigger (" + ws::itos(_actualChunk.content.size()) + ") "
			+ "than the expected size (" + ws::itos(_actualChunk.size) + ")");
		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));
		return false;
	}
	return true;
}

// ############## PUBLIC ##############

int ChunkedDataDecoder::decodeInto(std::vector<char> &vec) {
	if (_actualChunk.size == -1) {
		int chunkEndPos = ws::pos_in_vec("\r\n", _tmp);

		if (chunkEndPos == -1) {
			ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "chunk size not found. waiting for incoming data...");
			return ws::DECODER_WAITING_FOR_RECV;
		} else if (chunkEndPos == 0) {
			_tmp.erase(_tmp.begin(), _tmp.begin() + 2);
			return ws::DECODER_CALL_AGAIN;
		}
		return readChunkSize(chunkEndPos);
	} else {
		std::vector<char>::size_type i = 0;
		size_t chunkSize = _actualChunk.size;
		
		for (; i < _tmp.size() && _sizeRead + i < chunkSize; i++)
			_actualChunk.content.push_back(_tmp[i]);
		_sizeRead += i;

		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", ws::itos(_actualChunk.size - _sizeRead) + " chars left to read.");
		
		if (_sizeRead > chunkSize) {
			ws::log(ws::LOG_LVL_ERROR, "[ChunkedDataDecoder] -",
				"error while reading a chunk, the chunk size is bigger (" + ws::itos(_actualChunk.content.size()) + ") "
				+ "than the expected size (" + ws::itos(_actualChunk.size) + ")");
			ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));
			return ws::DECODER_CHUNKED_CHUNK_TOO_BIG;
		} else if (_sizeRead == chunkSize) {
			vec.insert(vec.end(), _actualChunk.content.begin(), _actualChunk.content.end());
			_tmp.erase(_tmp.begin(), _tmp.begin() + i + (i + 2 >= _tmp.size() ? 0 : 2));
			ws::log(ws::LOG_LVL_ALL, "[ChunkedDataDecoder] -", "a chunk of " + ws::itos(_actualChunk.size) + " chars was parsed");
			ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));
			
			clearActualChunk();
			return ws::DECODER_PARSE_READY;
		} else {
			ws::log(ws::LOG_LVL_ALL, "[ChunkedDataDecoder] -", ws::itos(i) + " were parsed");
			_tmp.erase(_tmp.begin(), _tmp.begin() + i);
			return ws::DECODER_WAITING_FOR_RECV;
		}
	}
}

ADataDecoder *ChunkedDataDecoder::clone() {
	return new ChunkedDataDecoder(*this);
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

ChunkedDataDecoder &ChunkedDataDecoder::operator=(ChunkedDataDecoder const &rhs) {
	if (this != &rhs) {
		_actualChunk = rhs._actualChunk;
		_sizeRead = rhs._sizeRead;
	}
	return *this;
}