#include "ChunkedBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ChunkedBody::ChunkedBody() {}
ChunkedBody::ChunkedBody(ChunkedBody const &chunkedBody) { *this = chunkedBody; }
ChunkedBody::~ChunkedBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void ChunkedBody::append(std::string str) {
	append(str, str.size());
}

void ChunkedBody::append(std::string str, int size) {
    t_block block = {size, str};
    _blocks.push_back(block);
    _totalSize += size;
}

int ChunkedBody::parse(std::string body, std::stringstream &inReceive) {
    static int hexSize = -1;
    size_t pos = body.find("\r\n");
    if (pos == std::string::npos) {
        ws::log(ws::LOG_LVL_DEBUG, "[CHUNKED BODY] -", "data stored in stringstream");
        inReceive << body;
        return 0;
    }

    if (hexSize < 0) {
        std::string hexStr = body.substr(0, pos);
        if (!ws::string_in_range(ws::HEX_VALUES, hexStr)) {
            ws::log(ws::LOG_LVL_ERROR, "[CHUNKED BODY] -", "error while reading chunk size. \"" + hexStr + "\" isn't a valid hex value!");
            return -1;
        }
        hexSize = ws::hextoi(hexStr);
        ws::log(ws::LOG_LVL_ALL, "[CHUNKED BODY] -", "a chunk of " + ws::itos(hexSize) + " chars is about to be parsed");

        if (hexSize != 0 && !body.erase(0, pos + 2).empty()) // deletes up to \r\n
            parse(body, inReceive);
        else if (hexSize == 0)
            hexSize = -2;
    } else {
        std::string chunkContent = body.substr(0, pos);
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

        inReceive << body.erase(0, pos + 2);
        hexSize = -1;
        // Recursively calls the function to parse the remaining body
        if (body.find("\r\n\r\n") != std::string::npos)
            parse(body, inReceive);
    }
    return hexSize == -2;
}

bool ChunkedBody::excecute() {}

// ############## GETTERS / SETTERS ##############

const std::string ChunkedBody::getBody() const {
	std::stringstream s;
	for (std::vector<t_block>::const_iterator it = _blocks.begin(); it != _blocks.end(); it++)
		s << it->content;
	return s.str();
}

const int &ChunkedBody::getSize() const {
	return _totalSize;
}

// ############## OPERATORS ##############

ChunkedBody &ChunkedBody::operator=(ChunkedBody const &rhs) {
	if (this != &rhs) {
		_blocks = rhs._blocks;
	}
	return *this;
}
