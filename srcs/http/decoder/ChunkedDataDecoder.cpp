#include "ChunkedDataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ChunkedDataDecoder::ChunkedDataDecoder() {
	clearActualChunk();
}

ChunkedDataDecoder::ChunkedDataDecoder(ChunkedDataDecoder const &chunkedDataDecoder) { *this = chunkedDataDecoder; }
ChunkedDataDecoder::~ChunkedDataDecoder() {}

// ############## PRIVATE ##############

void ChunkedDataDecoder::clearActualChunk() {
	_actualChunk.size = -1;
	_actualChunk.content.clear();
}

int ChunkedDataDecoder::readChunkSize(std::string const &buffer, size_t const &endChunkSizePos) {
	std::string sizeStr = buffer.substr(0, endChunkSizePos);
	if (!ws::string_in_range(ws::HEX_VALUES, sizeStr)) {
		ws::log(ws::LOG_LVL_ERROR, "[ChunkedDataDecoder] -", "error while reading chunk size. \"" + sizeStr + "\" isn't a valid hex value!");
		return -1;
	}

	_actualChunk.size = ws::hextoi(sizeStr);
	ws::log(ws::LOG_LVL_ALL, "[ChunkedDataDecoder] -", "a chunk of " + ws::itos(_actualChunk.size) + " chars is about to be parsed");

	if (_actualChunk.size != 0) {
		_actualChunk.content.reserve(_actualChunk.size);
		if (buffer.size() > endChunkSizePos + 2) // + 2 bc of \r\n
			return 2; // there's the body after
		return 1; // there's no body after
	} else
		return 0; // all chunks were read
}

bool ChunkedDataDecoder::checkChunkSize() {
	if (_actualChunk.content.size() > _actualChunk.size) {
		ws::log(ws::LOG_LVL_ERROR, "[ChunkedDataDecoder] -",
			"error while reading a chunk, the chunk size is bigger (" + ws::itos(_actualChunk.content.size()) + ")"
			+ "than the expected size (" + ws::itos(_actualChunk.size));
		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));
		return false;
	}
	return true;
}

// ############## PUBLIC ##############

int ChunkedDataDecoder::decodeInto(char *buffer, int size, std::vector<char> &vec) {
	if (_actualChunk.size == -1) {
		std::string wholeBuffer = ADataDecoder::bufferWithTmp(buffer, size);
		size_t endChunkSizePos = wholeBuffer.find("\r\n");

		if (endChunkSizePos == std::string::npos) {
            ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "chunk size not complete. stored in string.");
			ADataDecoder::fillTmp(buffer, size);
			return 0;
		}

		if (readChunkSize(wholeBuffer, endChunkSizePos) == 2) {
			size = wholeBuffer.size() - (endChunkSizePos + 2);
			decodeInto(ws::char_array(wholeBuffer, size, endChunkSizePos), size, vec);
		}
		_tmp.clear();
	} else {
		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", ws::itos(size) + " stored, " + ws::itos(size - _actualChunk.size) + " left to read.");
		for (int i = 0; i < size; i++)
			_actualChunk.content.push_back(buffer[i]);
		
		if (!checkChunkSize())
			return -2;
		else if (_actualChunk.content.size() == _actualChunk.size) {
			vec.insert(vec.end(), _actualChunk.content.begin(), _actualChunk.content.end());

			ws::log(ws::LOG_LVL_ALL, "[ChunkedDataDecoder] -", "a chunk of " + ws::itos(_actualChunk.size) + " chars was parsed");
			ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));

			int endChunk = _actualChunk.size + 2;
			clearActualChunk();
			decodeInto(buffer + endChunk, size - endChunk, vec);
		}
	}
	return _actualChunk.size == 0;
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

ChunkedDataDecoder &ChunkedDataDecoder::operator=(ChunkedDataDecoder const &rhs) {
	if (this != &rhs) {
		_actualChunk = rhs._actualChunk;
	}
	return *this;
}
