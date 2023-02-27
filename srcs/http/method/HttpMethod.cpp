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

	if (queryStartPos != std::string::npos) {
		data.cgiQuery.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
		data.clientPath = request.getPath().substr(0, queryStartPos);
	} else
		data.clientPath = request.getPath();

	data.location = Location::getBestMatch(data.clientPath, serverInfo);
	rootPath = data.location.getRootPath();
	indexPath = data.location.getIndexPath();

	std::cout << "root=" << rootPath << std::endl;
	std::cout << "index=" << indexPath << std::endl;
	std::cout << "clientPath=" << data.clientPath << std::endl;

	if (data.clientPath[data.clientPath.size() - 1] == '/')
		data.requestedPath = rootPath + data.clientPath + indexPath;
	else
		data.requestedPath = rootPath + data.clientPath;

	std::cout << "requested=" << data.requestedPath << std::endl;

	data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/") + 1, queryStartPos);
	if (data.fileName != "/" && data.fileName.find(".") != std::string::npos)
		data.fileExtension = data.fileName.substr(data.fileName.rfind(".") + 1, queryStartPos);
	std::cout << "ext=" << data.fileExtension << std::endl;
	std::cout << "name=" << data.fileName << std::endl;
	std::cout << "-----------------" << std::endl;
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
