#include "ChunkedBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ChunkedBody::ChunkedBody() : _totalSize(0) {}
ChunkedBody::ChunkedBody(ChunkedBody const &chunkedBody) { *this = chunkedBody; }
ChunkedBody::~ChunkedBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void ChunkedBody::append(std::string str, int size) {
    _totalSize += size;
    _body.append(str);
}

int ChunkedBody::parse(std::string body, std::stringstream &inReceive, int const &bodySize) {
    static int hexSize = -1;

    if (hexSize < 0) {
        size_t pos = body.find("\r\n");
        if (pos == std::string::npos) {
            ws::log(ws::LOG_LVL_DEBUG, "[CHUNKED BODY] -", "chunk size not complete. stored in stringstream.");
            inReceive << body;
            return 0;
        }

        std::string hexStr = body.substr(0, pos);
        if (!ws::string_in_range(ws::HEX_VALUES, hexStr)) {
            ws::log(ws::LOG_LVL_ERROR, "[CHUNKED BODY] -", "error while reading chunk size. \"" + hexStr + "\" isn't a valid hex value!");
            return -1;
        }

        hexSize = ws::hextoi(hexStr);
        ws::log(ws::LOG_LVL_ALL, "[CHUNKED BODY] -", "a chunk of " + ws::itos(hexSize) + " chars is about to be parsed");

        if (hexSize != 0 && !body.erase(0, pos + 2).empty()) // deletes up to \r\n
            parse(body, inReceive, bodySize);
        else if (hexSize == 0)
            hexSize = -2;
    } else {
        if (body.size() < hexSize) {
            ws::log(ws::LOG_LVL_DEBUG, "[CHUNKED BODY] -", "chunk contents not complete. stored in stringstream. " + ws::itos(hexSize - body.size()) + " chars needed.");
            inReceive << body;
            return 0;
        }
        std::string chunkContent = body.substr(0, hexSize);
        if (chunkContent.size() > hexSize) {
            ws::log(ws::LOG_LVL_ERROR, "[CHUNKED BODY] -",
                "error while reading a chunk, the chunk size is bigger (" + ws::itos(chunkContent.size()) + ")"
                + "than the given size (" + ws::itos(hexSize));
            ws::log(ws::LOG_LVL_DEBUG, "[CHUNKED BODY] -", "contents:\n" + chunkContent);

            return -2;
        }
        append(chunkContent, hexSize);
        ws::log(ws::LOG_LVL_ALL, "[CHUNKED BODY] -", "a chunk of " + ws::itos(hexSize) + " chars was parsed");
        ws::log(ws::LOG_LVL_DEBUG, "[CHUNKED BODY] -", "contents:\n" + chunkContent);

        inReceive << body.erase(0, hexSize + 2);
        hexSize = -1;
        // Recursively calls the function to parse the remaining body
        if (body.find("\r\n\r\n") != std::string::npos)
            parse(body, inReceive, bodySize);
    }
    return hexSize == -2;
}

void ChunkedBody::truncBody(int pos, int npos) {
    _body.erase(pos, npos);
    _totalSize -= npos;
}

// ############## GETTERS / SETTERS ##############

const std::string &ChunkedBody::getBody() const {
	return _body;
}

const int &ChunkedBody::getSize() const {
	return _totalSize;
}

// ############## OPERATORS ##############

ChunkedBody &ChunkedBody::operator=(ChunkedBody const &rhs) {
	if (this != &rhs) {
		_body = rhs._body;
		_totalSize = rhs._totalSize;
	}
	return *this;
}
