#include "HttpMethod.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMethod::HttpMethod() {}
HttpMethod::HttpMethod(HttpMethod const &httpMethod) { *this = httpMethod; }
HttpMethod::~HttpMethod() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

HttpMethod::request_data_t HttpMethod::initRequestData(ServerInfo const &serverInfo, HttpRequest const &request) {
	request_data_t data;

	if (request.getPath()[0] == '/' && request.getPath() != serverInfo.getRootPath()) {
		data.requestedFilePath = serverInfo.getRootPath() + request.getPath();

		if (data.requestedFilePath == serverInfo.getRootPath() + "/") // si request = / ; req -> index_path
			data.requestedFilePath = serverInfo.getRootPath() + serverInfo.getIndexPath();

		data.fileName = data.requestedFilePath.substr(data.requestedFilePath.rfind("/"));
		data.fileExtension = data.fileName.substr(data.fileName.rfind("."));
	}
	return data;
}

// ############## PUBLIC ##############

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpMethod &HttpMethod::operator=(HttpMethod const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
