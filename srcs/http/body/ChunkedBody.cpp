#include "ChunkedBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ChunkedBody::ChunkedBody() : _hexSize(-1) {}
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
}

int ChunkedBody::parse(std::string body) {
    body = _inReceive.str() + body;
    _inReceive.str("");
    size_t pos = body.find("\r\n");
    if (_hexSize == -1) {
        if (pos == std::string::npos)
            _inReceive << body;
        else {
            std::string hexStr = body.substr(0, pos);
            if (!ws::string_in_range(HEX_VALUES, hexStr)) {
                std::cerr << "error while reading chunk size. \"" << hexStr << "\" isn't a valid hex value." << std::endl;
                return -1;
            }
            _hexSize = ws::hextoi(hexStr);
            if (_hexSize != 0 && !body.erase(0, pos + 2).empty()) // deletes up to \r\n
                parse(body);
        }
    } else {
        if (pos == std::string::npos)
            _inReceive << body;
        else {
            pos = body.find("\r\n");
            std::string chunkContent = body.substr(0, pos);
            if (chunkContent.size() > _hexSize) {
                std::cerr << "error while reading chunk : \"" << chunkContent << "\" the chunk size is bigger than the given size (" << _hexSize << ")." << std::endl;
                return -2;
            }
            append(chunkContent, _hexSize);
            _inReceive << body.erase(0, pos + 2);
            _hexSize = -1;
            // just in case we received the 0 and parseRequest won't be called again bc recv read everything.
            if (body.find("\r\n\r\n") != std::string::npos)
                parse("");
        }
    }
    return _hexSize == 0;
}

// ############## GETTERS / SETTERS ##############

std::string ChunkedBody::getBody() const {
	std::stringstream s;
	for (std::vector<t_block>::const_iterator it = _blocks.begin(); it != _blocks.end(); it++)
		s << it->content;
	return s.str();
}

int ChunkedBody::getSize() const {
	int size;
	for (std::vector<t_block>::const_iterator it = _blocks.begin(); it != _blocks.end(); it++)
		size += it->size;
	return size;
}

// ############## OPERATORS ##############

ChunkedBody &ChunkedBody::operator=(ChunkedBody const &rhs) {
	if (this != &rhs) {
		_blocks = rhs._blocks;
        _inReceive << rhs._inReceive.str();
        _hexSize = rhs._hexSize;
	}
	return *this;
}
