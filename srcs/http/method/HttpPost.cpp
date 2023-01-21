#include "HttpPost.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpPost::HttpPost() {}
HttpPost::HttpPost(HttpPost const &httpPost) { *this = httpPost; }
HttpPost::~HttpPost() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

HttpResponse HttpPost::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	ws::log(ws::LOG_LVL_INFO, "[HTTP POST] -", "executing post request.");
	HttpResponse response;
	FileBody *fileBody = dynamic_cast<FileBody*>(request.getMessageBody());
	if (fileBody != NULL) {
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
