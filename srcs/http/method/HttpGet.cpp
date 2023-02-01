#include "HttpGet.hpp"
# include "CGI.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpGet::HttpGet() {}
HttpGet::HttpGet(HttpGet const &httpGet) { *this = httpGet; }
HttpGet::~HttpGet() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############



// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpGet &HttpGet::operator=(HttpGet const &rhs) {
	if (this != &rhs) {

	}
	return *this;
}

HttpResponse HttpGet::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	HttpResponse response;
	std::ifstream fileStream;
	bool autoIndex = serverInfo.hasAutoindex();

	DefaultBody *body = new DefaultBody();

	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	if (!ws::file_exists(data.requestedPath) && ws::file_is_dir(data.rawRequestedPath)) {
	for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
		size_t foundPos = data.rawRequestedPath.find(it->first);
		if (foundPos != std::string::npos) {
			if (std::find(it->second->getMethod().begin(), it->second->getMethod().end(), getName()) == it->second->getMethod().end())
				_errorCode = 405;
			break ;
		}
	}
}

	if (ws::file_is_dir(data.requestedPath)) {
		bool autoIndex = serverInfo.hasAutoindex();

		for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
			if (data.rawRequestedPath.find(it->first) != std::string::npos) {
				autoIndex = it->second->hasAutoindex();
				break ;
			}
		}
		if (autoIndex) {
			std::string autoIndexStr = ws::html_list_dir(data.rawRequestedPath);
			body->append(autoIndexStr, autoIndexStr.size());
		} else
			autoIndex = false;
	} else if (serverInfo.getCgis().count(data.fileExtension) != 0) {
		Cgi *cgi = new Cgi(serverInfo.getCgis());
		std::string responseReturn = cgi->execute(request, data, response);

		if (responseReturn != "error")
			body->append(responseReturn, responseReturn.size());
		else
			response.generateError(response.getStatusCode(), serverInfo.getErrorPages(), *body);
		delete cgi;
	}

	if (response.getStatusCode() < 400 && !autoIndex) {
		fileStream.open(data.requestedPath.c_str());

		if (!fileStream.is_open() || !ws::file_is_reg(data.requestedPath))
			response.generateError(404, serverInfo.getErrorPages(), *body);
		else {
			data.fileSize = ws::get_file_size(fileStream);

			body->append(ws::get_file_contents(fileStream, data.fileSize), data.fileSize);

			if (data.fileExtension == ".css")
				response.addHeader("Content-Type", "text/css");
			else if (data.fileExtension == ".html")
				response.addHeader("Content-Type", "text/html");
			else if (data.fileExtension == ".ico")
				response.addHeader("Content-Type", "text/favicon");
		}
		fileStream.close();
	}

	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());

	response.setMessageBody(body);

	return response;
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}
