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

	DefaultBody *body = new DefaultBody();

	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	if (ws::file_is_dir(data.requestedPath)) {
		bool autoIndex = serverInfo.hasAutoindex();

		for (std::map<std::string, Location*>::const_iterator it = serverInfo.getLocations().begin(); it != serverInfo.getLocations().end(); it++) {
			size_t foundPos = data.rawRequestedPath.find(it->first);
			if (foundPos != std::string::npos) {
				autoIndex = it->second->hasAutoindex();
				break ;
			}
		}

		std::string autoIndexStr = ws::html_list_dir(data.requestedPath);
		body->append(autoIndexStr, autoIndexStr.size());
	} else if (serverInfo.getCgis().count(data.fileExtension) != 0) {
		Cgi *cgi = new Cgi(serverInfo.getCgis());
		std::string responseReturn = cgi->execute(request, data, response);

		if (responseReturn != "error")
			body->append(responseReturn, responseReturn.size());
		else
			response.generateError(response.getStatusCode(), serverInfo.getErrorPages(), *body);
		delete cgi;
	}

	if (response.getStatusCode() < 400) {
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





