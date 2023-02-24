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
	
	std::string rootPath = serverInfo.getRootPath();
	std::string indexPath = serverInfo.getIndexPath();
	size_t queryStartPos = request.getPath().find('?');
	const Location *location;

	if (queryStartPos != std::string::npos) {
		data.cgiQuery.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
		data.clientPath = request.getPath().substr(0, queryStartPos);
	} else
		data.clientPath = request.getPath();

	if ((location = Location::getBestMatch(data.clientPath, serverInfo.getLocations())) != NULL) {
		std::cout << "root: " << location->getRootPath() << std::endl;
		rootPath = location->getRootPath();
		indexPath = location->getIndexPath();
	}

	std::cout << "root=" << rootPath << std::endl;
	std::cout << "index=" << indexPath << std::endl;
	std::cout << "clientPath=" << data.clientPath << std::endl;
	std::cout << "-----------------" << std::endl;

	if (data.clientPath[data.clientPath.size() - 1] == '/')
		data.requestedPath = rootPath + data.clientPath + indexPath;

	std::cout << "requested=" << data.requestedPath << std::endl;
	std::cout << "ext=" << data.fileExtension << std::endl;
	std::cout << "name=" << data.fileName << std::endl;

	// data.fileName = data.clientPath.substr(data.clientPath.rfind("/"), queryStartPos);
	// if (data.fileName != "/" && data.fileName.find(".") != std::string::npos)
	// 	data.fileExtension = data.fileName.substr(data.fileName.rfind("."), queryStartPos);
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
