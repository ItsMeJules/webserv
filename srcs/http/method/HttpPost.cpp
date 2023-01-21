#include "HttpPost.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpPost::HttpPost() {}
HttpPost::HttpPost(HttpPost const &httpPost) { *this = httpPost; }
HttpPost::~HttpPost() {}

// ############## PRIVATE ##############

bool HttpPost::isFile(HttpRequest const &request) const {
	return request.headersHasKey("Content-Type")
                && request.getHeader("Content-Type").rfind("multipart/form-data", 0) != std::string::npos;
}

void HttpPost::initFile(std::string const &header, std::string const &body) {
    _fileInfo.boundary = header.substr(header.find("boundary=") + 9);
    _fileInfo.fileHeader = body.substr(body.find((_fileInfo.boundary)) + _fileInfo.boundary.size() + 2, body.find("\r\n\r\n") - _fileInfo.boundary.size() - 4);
    size_t fileNamePos = _fileInfo.fileHeader.find("filename=") + 10;
    _fileInfo.fileName = _fileInfo.fileHeader.substr(fileNamePos, _fileInfo.fileHeader.size() - fileNamePos - 1);
}

// ############## PUBLIC ##############

HttpResponse HttpPost::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	ws::log(ws::LOG_LVL_INFO, "[HTTP POST] -", "executing post request.");
	HttpResponse response;
	FileBody *fileBody = dynamic_cast<FileBody*>(request.getMessageBody());
	if (isFile(request)) {
		ws::log(ws::LOG_LVL_DEBUG, "[HTTP POST] -", "the request is a file post, initializing file...");
		initFile(request.getHeader("Content-Type"), request.getMessageBody()->getBody());
		ws::log(ws::LOG_LVL_DEBUG, "[HTTP POST] -", "creating file " + serverInfo.getRootPath() + serverInfo.getUploadPath() + fileBody->getFileName());
		fileBody->createFile(serverInfo.getRootPath() + serverInfo.getUploadPath());
		response.setStatusCode(fileBody->fileExists() ? 201 : 500);
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
