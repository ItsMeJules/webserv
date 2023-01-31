#include "HttpMethod.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMethod::HttpMethod() {}
HttpMethod::HttpMethod(HttpMethod const &httpMethod) { *this = httpMethod; }
HttpMethod::~HttpMethod() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

ws::request_data_t HttpMethod::initRequestData(ServerInfo const &serverInfo, HttpRequest const &request, std::string const &requestType) {
	ws::request_data_t data;
	size_t queryStartPos = request.getPath().find('?');

	if (queryStartPos != std::string::npos) {
		data.query.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
		data.requestedPath = request.getPath().substr(0, queryStartPos);
	}

	data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/"), queryStartPos);
	data.fileExtension = data.fileName.substr(data.fileName.rfind("."), queryStartPos);

	for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
		if (request.getPath() != it->first)
			continue ;
		data.requestedPath = it->second->getRootPath();
		return data;
	}
	
	data.requestedPath = serverInfo.getRootPath() + data.requestedPath;
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
