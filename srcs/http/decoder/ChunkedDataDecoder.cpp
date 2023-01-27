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
	_sizeRead = 0;
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


bool ChunkedDataDecoder::checkChunkSize(const char *buffer, int const &bufSize, int const &read) {
	if (read != bufSize && read + 2 <= bufSize && (buffer[read] != '\r' && buffer[read + 1] != '\n')) {
		ws::log(ws::LOG_LVL_ERROR, "[ChunkedDataDecoder] -",
			"error while reading a chunk, the chunk size is bigger (" + ws::itos(_actualChunk.content.size()) + ") "
			+ "than the expected size (" + ws::itos(_actualChunk.size) + ")");
		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));
		return false;
	}
	return true;
}

// ############## PUBLIC ##############

# include <iostream>

int ChunkedDataDecoder::decodeInto(char *buffer, int size, std::vector<char> &vec) {
	if (_actualChunk.size == -1) {
		std::string wholeBuffer = ADataDecoder::bufferWithTmp(buffer, size);
		size_t endChunkSizePos = wholeBuffer.find("\r\n");

		if (endChunkSizePos == 0) { // if the found pos is 0, it means there are no number so we skip the position.
			ADataDecoder::fillTmp(buffer + 2, size - 2);
            ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "chunk size not complete. stored in string.");
			return 0;
		} else if (endChunkSizePos == std::string::npos) {
            ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "chunk size not complete. stored in string.");
			ADataDecoder::fillTmp(buffer, size);
			return 0;
		}

		if (readChunkSize(wholeBuffer, endChunkSizePos) == 2) {
			buffer = ws::char_array(wholeBuffer, wholeBuffer.size(), endChunkSizePos + 2);
			decodeInto(buffer, wholeBuffer.size() - (endChunkSizePos + 2), vec);
			delete buffer;
		}
		_tmp.clear();
	} else if (_actualChunk.size != 0) {
		int i = 0;
		for (i = 0; i < size && _actualChunk.content.size() != _actualChunk.size; i++)
			_actualChunk.content.push_back(buffer[i]);
		
		_sizeRead += i;
		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", ws::itos(i) + " chars stored, " + ws::itos(_actualChunk.size - _sizeRead) + " left to read.");

		if (!checkChunkSize(buffer, size, i))
			return -2;
		else if (_actualChunk.content.size() == _actualChunk.size) {
			vec.insert(vec.end(), _actualChunk.content.begin(), _actualChunk.content.end());
			ws::log(ws::LOG_LVL_ALL, "[ChunkedDataDecoder] -", "a chunk of " + ws::itos(_actualChunk.size) + " chars was parsed");
			ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));

			if (i != size)
				i += 2;

			buffer = ws::char_array(std::string(buffer, size), size, i);
			_previousChunk = _actualChunk;

			clearActualChunk();
			decodeInto(buffer, size - i, vec);
			delete buffer;
		}
	}
	if (_actualChunk.size == 0) {
		if (_previousChunk.size != 0) {
			_previousChunk.size = 0;
			std::cout << "lol" << std::endl;
			return 2;
		}
		std::cout << "mdr: " << _actualChunk.size << std::endl;
		ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "all chunks were received & parsed!");
		return 1;
	} else
		return 2;
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