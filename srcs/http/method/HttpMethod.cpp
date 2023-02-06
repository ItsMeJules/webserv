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
	
	std::string rootPath = serverInfo.getRootPath() + '/';
	std::string indexPath = serverInfo.getIndexPath();
	size_t queryStartPos = request.getPath().find('?');

	if (queryStartPos != std::string::npos) {
		data.cgiQuery.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
		data.clientPath = request.getPath().substr(0, queryStartPos);
	} else
		data.clientPath = request.getPath();

	for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
		if (data.clientPath == it->first)
			continue ;

		rootPath = it->second->getRootPath() + it->first + '/';
		indexPath = it->second->getIndexPath();
	}

	if (rootPath + data.clientPath == rootPath + indexPath)
		data.requestedPath = rootPath + data.clientPath;
	else
		data.requestedPath = rootPath + data.clientPath + indexPath;

	std::cout << "requested=" << data.requestedPath << std::endl;
	std::cout << "ext=" << data.fileExtension << std::endl;
	std::cout << "name=" << data.fileName << std::endl;

	// data.fileName = data.clientPath.substr(data.clientPath.rfind("/"), queryStartPos);
	// if (data.fileName != "/" && data.fileName.find(".") != std::string::npos)
	// 	data.fileExtension = data.fileName.substr(data.fileName.rfind("."), queryStartPos);
	return data;
}

// ws::request_data_t HttpMethod::initRequestData(ServerInfo const &serverInfo, HttpRequest const &request) {
// 	ws::request_data_t data;
// 	std::string root = serverInfo.getRootPath();
// 	std::string indexPath = serverInfo.getIndexPath();
// 	size_t queryStartPos = request.getPath().find('?');

// 	if (root[root.length()] == '/' && request.getPath()[0] == '/')
//         root.erase(0, 1);

// 	if (queryStartPos != std::string::npos) {
// 		data.query.assign(request.getPath(), queryStartPos + 1, request.getPath().size());
// 		data.rawRequestedPath = root + request.getPath().substr(0, queryStartPos);
// 	} else
// 		data.rawRequestedPath = root + request.getPath();

// 	data.requestedPath = data.rawRequestedPath;
// 	for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
// 		size_t foundPos = data.rawRequestedPath.find(it->first);
// 		if (foundPos == std::string::npos)
// 			continue ;

// 		root = it->second->getRootPath();
// 		data.requestedPath.erase(foundPos, it->first.size());
// 		data.requestedPath = it->second->getRootPath() + data.requestedPath;
// 		if (data.requestedPath == root + "/")
// 			indexPath = it->second->getIndexPath();
// 	}

// 	data.rawRequestedPath = data.requestedPath;
// 	// if (data.requestedPath == root + "/")
// 	// 	data.requestedPath = indexPath;

// 	data.fileName = data.requestedPath.substr(data.requestedPath.rfind("/"), queryStartPos);
// 	if (data.fileName != "/" && data.fileName.find(".") != std::string::npos)
// 		data.fileExtension = data.fileName.substr(data.fileName.rfind("."), queryStartPos);
// 	return data;
// }

// ############## PUBLIC ##############

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpMethod &HttpMethod::operator=(HttpMethod const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
