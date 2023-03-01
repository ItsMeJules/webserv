#include "HttpMethod.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMethod::HttpMethod() {}
HttpMethod::HttpMethod(HttpMethod const &httpMethod) { *this = httpMethod; }
HttpMethod::~HttpMethod() {}

// ############## PRIVATE ##############

bool HttpMethod::isValid(std::ifstream const &fileStream, ws::request_data_t &data, ServerInfo const &serverInfo, DefaultBody *body, HttpResponse &response) {
	if (!fileStream.is_open()) {
		if (!data.indexAppended) {
			if (data.location.getAutoindex() == 0 || (data.location.getAutoindex() == 1 && !ws::file_is_dir(data.requestedPath))) {
				response.generateError(404, serverInfo, *body);
				return false;
			}
		} else
			data.requestedPath.erase(data.requestedPath.size() - data.location.getIndexPath().size());
	}

	const std::vector<std::string> allowedMethods = data.location.getMethod();

	if (std::find(allowedMethods.begin(), allowedMethods.end(), getName()) == allowedMethods.end()) {
		response.generateError(405, serverInfo, *body);
		return false;
	}
	return true;
}


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
			data.requestedPath += "/";
		data.requestedPath += data.location.getIndexPath();
		data.indexAppended = true;
	} else
		data.indexAppended = false;

	std::cout << "requested=" << data.requestedPath << std::endl;
	std::cout << "index=" << data.location.getIndexPath() << std::endl;
	std::cout << "root=" << data.location.getRootPath() << std::endl;
	std::cout << "client=" << data.clientPath << std::endl;

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
