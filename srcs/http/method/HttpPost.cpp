#include "HttpPost.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpPost::HttpPost() {}
HttpPost::HttpPost(HttpPost const &httpPost) { *this = httpPost; }
HttpPost::~HttpPost() {}

// ############## PRIVATE ##############

const bool HttpPost::isFile(HttpRequest const &request) const {
	return request.headersHasKey("Content-Type")
                && request.getHeader("Content-Type").rfind("multipart/form-data", 0) != std::string::npos;
}

const bool HttpPost::fileExists() const {
    struct stat buffer;   
    return stat(_fileInfo.filePath.c_str(), &buffer) == 0; 
}

void HttpPost::getFileInfo(std::string const &header, std::string const &body) {
	ws::log(ws::LOG_LVL_DEBUG, "[HTTP POST] -", "the request is a file post, initializing file...");
	
    _fileInfo.boundary = header.substr(header.find("boundary=") + 9);
    _fileInfo.fileHeader = body.substr(body.find((_fileInfo.boundary)) + _fileInfo.boundary.size() + 2, body.find("\r\n\r\n") - _fileInfo.boundary.size() - 4);
    size_t fileNamePos = _fileInfo.fileHeader.find("filename=") + 10;
    _fileInfo.fileName = _fileInfo.fileHeader.substr(fileNamePos, _fileInfo.fileHeader.size() - fileNamePos - 1);
    ws::log(ws::LOG_LVL_ALL, "[HTTP POST] -", "file: \"" + _fileInfo.fileName + "\" was parsed.");
}

const bool HttpPost::createFile(std::string const &path, const IMessageBody *const body) {
    _fileInfo.filePath = std::string(path + (path.at(path.size() - 1) != '/' ? "" : "/") + _fileInfo.fileName);
    int fd = ::open(_fileInfo.filePath.c_str(), O_CREAT | O_RDWR);

    chmod(_fileInfo.filePath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, body->getBody().c_str(), body->getSize());
	ws::log(ws::LOG_LVL_INFO, "[HTTP POST] -", ws::itos(body->getSize()) + " chars written.");
    ::close(fd);
    return true;
}

// ############## PUBLIC ##############

HttpResponse HttpPost::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	ws::log(ws::LOG_LVL_INFO, "[HTTP POST] -", "executing post request.");
	HttpResponse response;
	
	if (isFile(request)) {
		getFileInfo(request.getHeader("Content-Type"), request.getMessageBody()->getBody());
		ws::log(ws::LOG_LVL_DEBUG, "[HTTP POST] -", "creating file " + serverInfo.getRootPath() + serverInfo.getUploadPath() + _fileInfo.fileName);
		createFile(serverInfo.getRootPath() + serverInfo.getUploadPath(), request.getMessageBody());
		response.setStatusCode(fileExists() ? 201 : 500);
	}
	return response;
}

HttpMethod *HttpPost::clone() {
	return new HttpPost();
}

std::string HttpPost::getName() {
	return "POST";
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpPost &HttpPost::operator=(HttpPost const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}
