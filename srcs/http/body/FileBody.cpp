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

int FileBody::parse(std::string body, std::stringstream &inReceive) {
    size_t endPos = body.find(_boundary + "--");
    if (endPos == std::string::npos) {
        inReceive << body;
        ws::log(ws::LOG_LVL_DEBUG, "[FILE BODY] -", "data stored in stringstream");
        return 0;
    }
    ws::log(ws::LOG_LVL_ALL, "[FILE BODY] -", "a file is about to be parsed");
    _fileHeader = body.substr(body.find((_boundary)) + _boundary.size() + 2, body.find("\r\n\r\n") - _boundary.size() - 4);
    size_t fileNamePos = _fileHeader.find("filename=") + 10;
    _fileName = _fileHeader.substr(fileNamePos, _fileHeader.size() - fileNamePos - 1);
    append(body.substr(body.find("\r\n\r\n") + 4, endPos - body.find("\r\n\r\n") - 8));
    ws::log(ws::LOG_LVL_ALL, "[FILE BODY] -", "file: \"" + _fileName + "\" was parsed");
    ws::log(ws::LOG_LVL_DEBUG, "", "contents:\n" + body);
    return 1;
}

// ############## GETTERS / SETTERS ##############

const std::string FileBody::getBody() const {
	return _contents.str();
}

const std::string &FileBody::getFileHeader() const {
    return _fileHeader;
}

const int &FileBody::getSize() const {
	return _size;
}

const std::string &FileBody::getFileName() const {
	return _fileName;
}

const std::string &FileBody::getBoundary() const {
    return _boundary;
}

void FileBody::setBoundary(std::string header) {
    _boundary = header.substr( header.find("boundary=") + 9);
}

// ############## OPERATORS ##############

FileBody &FileBody::operator=(FileBody const &rhs) {
	if (this != &rhs) {
        _contents << rhs._contents.str();
        _fileHeader = rhs._fileHeader;
        _fileName = rhs._fileName;
        _boundary = rhs._boundary;
        _size = rhs._size;
	}
	return *this;
}
