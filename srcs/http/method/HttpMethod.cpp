#include "HttpMethod.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMethod::HttpMethod() {}
HttpMethod::HttpMethod(HttpMethod const &httpMethod) { *this = httpMethod; }
HttpMethod::~HttpMethod() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

HttpMethod::request_data_t HttpMethod::initRequestData(ServerInfo const &serverInfo, HttpRequest const &request, std::string const &requestType) {
	request_data_t data;

	if (request.getPath() != serverInfo.getRootPath()) {
		data.requestedPath = serverInfo.getRootPath();

		if (requestType == "GET") {
			data.requestedPath += request.getPath();
			
			if (data.requestedPath == serverInfo.getRootPath() + "/") // si request == root_path ; req -> index_path
				data.requestedPath += serverInfo.getIndexPath();

			data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/"));
			data.fileExtension = data.fileName.substr(data.fileName.rfind("."));
		}
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
