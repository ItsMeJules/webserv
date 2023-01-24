#include "FileBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

FileBody::FileBody(IMessageBody *parsingType, std::string boundaryHeader) :
	_parsingType(parsingType),
	_boundary(boundaryHeader.substr(boundaryHeader.find("boundary=") + 9)) {}

FileBody::FileBody() : _parsingType(NULL) {}
FileBody::FileBody(FileBody const &fileBody) { *this = fileBody; }

FileBody::~FileBody() {
	delete _parsingType;
}

// ############## PRIVATE ##############
// ############## PUBLIC ##############

void FileBody::append(std::string str, int size) {

}

int FileBody::parse(std::string body, std::stringstream &inReceive, int const &bodySize) {
	size_t metadataPos;
	t_form_data data;
	std::string _fileHeader;
	
	if (_parsingType->parse(body, inReceive, bodySize) == 1) {
		ws::log(ws::LOG_LVL_DEBUG, "[FILE BODY] -", "the request is a file post, initializing file...");

		while ((metadataPos = getBody().find("--" + _boundary + "\r\n")) != std::string::npos) {
			size_t endHeaderPos = getBody().find("\r\n\r\n");
			_fileHeader = getBody().substr(metadataPos + _boundary.size() + 4, endHeaderPos - _boundary.size() - 4);

			if (_fileHeader.find("filename=") != std::string::npos) { // find 2x
				size_t fileNamePos = _fileHeader.find("filename=") + 10;
				_fileName = _fileHeader.substr(fileNamePos, _fileHeader.size() - fileNamePos - 1);
				ws::log(ws::LOG_LVL_INFO, "[FILE BODY] -", "reading file contents...");
				truncBody(0, endHeaderPos + 4);
				break ;
			}

			size_t keyPos = _fileHeader.find("name=") + 6;
			data.name = _fileHeader.substr(keyPos, _fileHeader.size() - keyPos - 1);

			size_t dataEndPos = getBody().substr(endHeaderPos + 4).find("\r\n");
			data.data = getBody().substr(endHeaderPos + 4, dataEndPos);
			
			ws::log(ws::LOG_LVL_DEBUG, "[FILE BODY] -", "data-name: \"" + data.name + "\" | data-value: \"" + data.data + "\"");
			_values.push_back(data);

			truncBody(0, getBody().find(data.data) + data.data.size() + 2);
		}
		truncBody(getBody().find("\r\n--" + _boundary + "--"), _boundary.size() + 8);

		ws::log(ws::LOG_LVL_ALL, "[FILE BODY] -", "file: \"" + _fileName + "\" was parsed.");
		return 1;
	}
	return 0;
}

void FileBody::truncBody(int pos, int npos) {
	_parsingType->truncBody(pos, npos);
}

const bool FileBody::fileExists() const {
    struct stat buffer;   
    return stat(_filePath.c_str(), &buffer) == 0; 
}

const bool FileBody::createFile(std::string const &path) {
    _filePath = std::string(path + (path.at(path.size() - 1) != '/' ? "/" : "") + _fileName);
    int fd = ::open(_filePath.c_str(), O_CREAT | O_RDWR);

    chmod(_filePath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, getBody().c_str(), getSize());
	ws::log(ws::LOG_LVL_INFO, "[FILE BODY] -", ws::itos(getSize()) + " chars written at " + _filePath);
    ::close(fd);
    return true;
}

// ############## GETTERS / SETTERS ##############

const std::string &FileBody::getBody() const {
	return _parsingType->getBody();
}

const int &FileBody::getSize() const {
	return _parsingType->getSize();
}

const std::string &FileBody::getFileName() const {
	return _fileName;
}

const std::string &FileBody::getBoundary() const {
    return _boundary;
}

const std::string &FileBody::getFilePath() const {
	return _filePath;
}

void FileBody::setFilePath(std::string filePath) {
	_filePath = filePath;
}

// ############## OPERATORS ##############

FileBody &FileBody::operator=(FileBody const &rhs) {
	if (this != &rhs) {
		_parsingType = rhs._parsingType;
	}
	return *this;
}
