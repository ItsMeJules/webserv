#include "FileBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

FileBody::FileBody() : _size(0) {}
FileBody::FileBody(FileBody const &fileBody) { *this = fileBody; }
FileBody::~FileBody() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void FileBody::append(std::string str) {
	append(str, str.size());
}

void FileBody::append(std::string str, int size) {
	_contents << str;
	_size += size;
}

// ############## GETTERS / SETTERS ##############

std::string FileBody::getBody() const {
	return _contents.str();
}

int FileBody::getSize() const {
	return _size;
}

std::string FileBody::getFileName() const {
	return _fileName;
}

// ############## OPERATORS ##############

FileBody &FileBody::operator=(FileBody const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}
