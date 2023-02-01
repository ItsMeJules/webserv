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

	if (root[root.length()] == '/' && request.getPath()[0] == '/')
        root.erase(0, 1);
		
	if (queryStartPos != std::string::npos) {
		data.query.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
		data.requestedPath = root + request.getPath().substr(0, queryStartPos);
	} else
		data.requestedPath = root + request.getPath();

	for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
		if (data.requestedPath != it->first)
			continue ;
		std::cout << "req_before: " << data.requestedPath << std::endl;
		data.requestedPath.erase(0, it->first.size());
		data.requestedPath = it->second->getRootPath() + data.requestedPath;
	}

	if (data.requestedPath == root + "/")
		data.requestedPath = serverInfo.getIndexPath();

	data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/"), queryStartPos);
	if (data.fileName != "/" && data.fileName.find(".") != std::string::npos)
		data.fileExtension = data.fileName.substr(data.fileName.rfind("."), queryStartPos);
	std::cout << "req:" << data.requestedPath << std::endl;
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
