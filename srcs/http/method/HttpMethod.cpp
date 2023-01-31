#include "HttpMethod.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMethod::HttpMethod() {}
HttpMethod::HttpMethod(HttpMethod const &httpMethod) { *this = httpMethod; }
HttpMethod::~HttpMethod() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

ws::request_data_t HttpMethod::initRequestData(ServerInfo const &serverInfo, HttpRequest const &request, std::string const &requestType) {
	ws::request_data_t data;

	if (request.getPath() != serverInfo.getRootPath()) {
		data.requestedPath = serverInfo.getRootPath();

		if (requestType == "GET") {
			size_t queryStartPos = request.getPath().find('?');

			if (queryStartPos != std::string::npos) {
				data.query.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
				data.requestedPath += request.getPath().substr(0, queryStartPos);
			} else
				data.requestedPath += request.getPath();

			if (data.requestedPath == serverInfo.getRootPath() + "/") // si request == root_path ; req -> index_path
				data.requestedPath += serverInfo.getIndexPath();

			data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/"), queryStartPos);
			data.fileExtension = data.fileName.substr(data.fileName.rfind("."), queryStartPos);
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
