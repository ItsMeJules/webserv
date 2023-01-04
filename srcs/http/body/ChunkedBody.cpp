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
	}
	return *this;
}
