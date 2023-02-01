#include "HttpMethod.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMethod::HttpMethod() {}
HttpMethod::HttpMethod(HttpMethod const &httpMethod) { *this = httpMethod; }
HttpMethod::~HttpMethod() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

ws::request_data_t HttpMethod::initRequestData(ServerInfo const &serverInfo, HttpRequest const &request) {
	ws::request_data_t data;
	std::string root = serverInfo.getRootPath();
	size_t queryStartPos = request.getPath().find('?');

	if (queryStartPos != std::string::npos) {
		data.query.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
		data.requestedPath = root + request.getPath().substr(0, queryStartPos);
		root = "";
	} else {
		data.requestedPath = root + request.getPath();
		root = "";
	}

	for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
		if (request.getPath() != it->first)
			continue ;

		root = it->second->getRootPath();
		data.requestedPath.erase(0, it->first.size());
	}

	if (data.requestedPath == "/")
		data.requestedPath = serverInfo.getIndexPath();

	data.requestedPath = root + data.requestedPath;
	data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/"), queryStartPos);
	if (data.fileName != "/" && data.fileName.find(".") != std::string::npos)
		data.fileExtension = data.fileName.substr(data.fileName.rfind("."), queryStartPos);

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
