#include "HttpGet.hpp"
# include "CGI.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpGet::HttpGet() {}
HttpGet::HttpGet(HttpGet const &httpGet) : HttpMethod(*this) { *this = httpGet; }
HttpGet::~HttpGet() {}


HttpGet &HttpGet::operator=(HttpGet const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}

HttpResponse HttpGet::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	HttpResponse response;
	std::ifstream fileStream;
	size_t fileSize;

	DefaultBody *body = new DefaultBody();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	fileStream.open(data.requestedPath.c_str());

	if (!fileStream.is_open()) {
		response.generateError(404, serverInfo.getErrorPages(), *body);
		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Date", response.generateDate());

		response.setMessageBody(body);
		return response;
	}

	const std::vector<std::string> allowedMethods = data.location.getMethod();

	if (std::find(allowedMethods.begin(), allowedMethods.end(), getName()) == allowedMethods.end()) {
		response.generateError(405, serverInfo.getErrorPages(), *body);
		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Date", response.generateDate());

		response.setMessageBody(body);
		return response;
	}

	if (ws::file_is_dir(data.requestedPath)) {
			response.generateError(404, serverInfo.getErrorPages(), *body);
		if (data.location.getAutoindex() == 0) {
			response.addHeader("Content-Length", ws::itos(body->getBodySize()));
			response.addHeader("Date", response.generateDate());

			response.setMessageBody(body);
			return response;
		}
		std::string autoIndexStr = ws::html_list_dir(data.requestedPath, data.clientPath);

		body->append(autoIndexStr, autoIndexStr.size());

		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Date", response.generateDate());

		response.setMessageBody(body);
		return response;
	}

	if (serverInfo.getCgis().count(data.fileExtension) != 0) {
		Cgi *cgi = new Cgi(serverInfo.getCgis());

		if (request.getMethod()->getName() != "GET") {
			if (request.headersHasKey("Content-Type"))
				cgi->setContentType(request.getHeader("Content-Type"));
			else {
				response.generateError(500, serverInfo.getErrorPages(), *body);
				response.addHeader("Content-Length", ws::itos(body->getBodySize()));
				response.addHeader("Date", response.generateDate());

				response.setMessageBody(body);
				return response;
			}
		}

		std::cerr << "********************CGI***********************" << std::endl;


		std::string responseReturn = cgi->execute(request, data);
		body->append(responseReturn, responseReturn.size());

		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Date", response.generateDate());

		response.setMessageBody(body);

		delete cgi;
		return response;
	}

	fileSize = ws::get_file_size(fileStream);

	body->append(ws::get_file_contents(fileStream, fileSize), fileSize);
	response.addHeader("Content-Type", ws::mimeTypeFromExtension(data.fileExtension));

	fileStream.close();

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
