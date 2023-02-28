#include "HttpMethod.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMethod::HttpMethod() {}
HttpMethod::HttpMethod(HttpMethod const &httpMethod) { *this = httpMethod; }
HttpMethod::~HttpMethod() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

// paths in config never ends with a '/' but they can start with a /
ws::request_data_t HttpMethod::initRequestData(ServerInfo const &serverInfo, HttpRequest const &request) {
	ws::request_data_t data;
	size_t queryStartPos = request.getPath().find('?');

	if (queryStartPos != std::string::npos) {
		data.cgiQuery.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
		data.clientPath = request.getPath().substr(0, queryStartPos);
	} else
		data.clientPath = request.getPath();

	data.location = Location::getBestMatch(data.clientPath, serverInfo);
	data.requestedPath = data.location.getRootPath() + data.clientPath;

	if (ws::file_is_dir(data.requestedPath)) {
		if (data.clientPath[data.clientPath.size() - 1] != '/')
			data.clientPath += "/";
		data.requestedPath = data.location.getRootPath() + data.clientPath + data.location.getIndexPath();
	}

	data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/") + 1, queryStartPos);
	if (data.fileName != "/" && data.fileName.find(".") != std::string::npos)
		data.fileExtension = data.fileName.substr(data.fileName.rfind(".") + 1, queryStartPos);
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
