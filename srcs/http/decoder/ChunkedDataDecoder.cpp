#include "ChunkedDataDecoder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ChunkedDataDecoder::ChunkedDataDecoder() {
	clearActualChunk();
}

ChunkedDataDecoder::ChunkedDataDecoder(ChunkedDataDecoder const &chunkedDataDecoder) { *this = chunkedDataDecoder; }
ChunkedDataDecoder::~ChunkedDataDecoder() {}

// ############## PRIVATE ##############
# include <iostream>

void ChunkedDataDecoder::clearActualChunk() {
	std::cout << "cleared" << std::endl;
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
		std::cout << "read: " << read << "bufsize: " << bufSize << std::endl;
		std::cout << "----------check-----------------" << std::endl;
		for (int i = 0; i < bufSize; i++)
			std::cout << buffer[i] << " " << +buffer[i] << " ";
		std::cout << std::endl;
		std::cout << "---------------------------" << std::endl;
		std::cout << buffer[read] << " " << +buffer[read] << " | " << buffer[read + 1] << " " << +buffer[read + 1] << std::endl;
		ws::log(ws::LOG_LVL_ERROR, "[ChunkedDataDecoder] -",
			"error while reading a chunk, the chunk size is bigger (" + ws::itos(_actualChunk.content.size()) + ") "
			+ "than the expected size (" + ws::itos(_actualChunk.size) + ")");
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

		std::cout << "----------whole buffer-----------------" << std::endl;
		std::cout << wholeBuffer << "|" << std::endl;
		std::cout << "---------------------------" << std::endl;
		if (endChunkSizePos == std::string::npos) {
            ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "chunk size not complete. stored in string.");
			ADataDecoder::fillTmp(buffer, size);
			return 0;
		}

		if (readChunkSize(wholeBuffer, endChunkSizePos) == 2) {
			std::cout << "endchunkSizePos: " << endChunkSizePos << std::endl;
			std::cout << "-------disp------------" << std::endl;
			for (int k = 0; k < size; k++)
				std::cout << "[" << wholeBuffer[k] << "]" << std::endl;
			std::cout << "-------disp------------" << std::endl;
			buffer = ws::char_array(wholeBuffer, wholeBuffer.size(), endChunkSizePos + 2);
			decodeInto(buffer, wholeBuffer.size() - (endChunkSizePos + 2), vec);
			delete buffer;
		}
		_tmp.clear();
	} else if (_actualChunk.size != 0) {
		int i = 0;
		std::cout << "size: " << size << " actual: " << _actualChunk.content.size() << std::endl;
		if (size == 96) {
			for (int k = 0; k < size; k++)
				std::cout << "[" << buffer[k] << "]" << std::endl;
		}
		std::cout << "----------stored-----------------" << std::endl;
		for (i = 0; i < size && _actualChunk.content.size() != _actualChunk.size; i++) {
			std::cout << buffer[i] << " " << +buffer[i] << " ";
			_actualChunk.content.push_back(buffer[i]);
		}
		std::cout << std::endl;
		std::cout << "---------------------------" << std::endl;
		std::cout << "----------not_stored-----------------" << std::endl;
		for (int j = i; j < size; j++)
			std::cout << buffer[j];
		std::cout << "---------------------------" << std::endl;
		
		_sizeRead += i;
		ws::log(ws::LOG_LVL_INFO, "[ChunkedDataDecoder] -", ws::itos(i) + " chars stored, " + ws::itos(_actualChunk.size - _sizeRead) + " left to read.");

		if (!checkChunkSize(buffer, size, i))
			return -2;
		else if (_actualChunk.content.size() == _actualChunk.size) {
			vec.insert(vec.end(), _actualChunk.content.begin(), _actualChunk.content.end());
			ws::log(ws::LOG_LVL_ALL, "[ChunkedDataDecoder] -", "a chunk of " + ws::itos(_actualChunk.size) + " chars was parsed");
			ws::log(ws::LOG_LVL_DEBUG, "[ChunkedDataDecoder] -", "contents:\n" + std::string(_actualChunk.content.data(), _actualChunk.content.size()));

			std::cout << "---------------------------" << std::endl;
			for (int i = 0; i < vec.size(); i++)
				std::cout << vec[i];
			std::cout << std::endl;
			std::cout << "---------------------------" << std::endl;

			if (i != size)
				i += 2;

			buffer = ws::char_array(std::string(buffer, size), size, i);

			std::cout << size << " " << i << std::endl;

			clearActualChunk();
			decodeInto(buffer, size - i, vec);
			delete buffer;
			return _actualChunk.size == 0 ? 1 : 2;
		}
	}
	return _actualChunk.size == 0;
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