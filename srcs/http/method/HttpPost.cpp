#include "HttpPost.hpp"
# include "CGI.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpPost::HttpPost() {}
HttpPost::HttpPost(HttpPost const &httpPost) { *this = httpPost; }
HttpPost::~HttpPost() {}

// ############## PRIVATE ##############

bool HttpPost::writePartToFile(FormDataBody::FormDataPart &part, std::string const &filePath, std::ofstream &ofs) {
	if (!ofs.is_open())
		ofs.open(filePath.c_str(), std::ofstream::binary | std::ofstream::out);

	if (ofs.is_open()) {
		ws::log(ws::LOG_LVL_DEBUG, "[HttpPost] -", "data wrote to " + filePath);
		ofs << part.extractBody();
		return true;
	}
	return false;
}

// ############## PUBLIC ##############

HttpResponse HttpPost::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	ws::log(ws::LOG_LVL_INFO, "[HTTP POST] -", "executing post request.");

	HttpResponse response;
	DefaultBody *body = new DefaultBody();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);
	FormDataBody *formBody = dynamic_cast<FormDataBody*>(request.getMessageBody());

	Cgi *cgi = new Cgi(serverInfo.getCgis());

	if (serverInfo.getCgis().count(data.fileExtension) == 1) {
		std::string responseRet = cgi->execute(request, data, response);
		
		if (responseRet != "error")
			body->append(responseRet, responseRet.size());
	}

	if (formBody != NULL) {
		FormDataBody::FormDataPart *part;
		std::ofstream ofs;

		bool success = true;
		data.requestedPath += serverInfo.getUploadPath();

		while ((part = formBody->readForm()) != NULL) {
			if (!part->_headersParsed || part->_directiveName != part->_fileKey)
				continue ;

			if (!writePartToFile(*part, data.requestedPath + "/" + part->_fileName, ofs)) {
				success = false;
				break ;
			}
		}
	}

	if (!ws::file_exists(data.requestedPath)) {
		response.setStatusCode(404);
		response.addHeader("Connection", "close");
	}

	body->append(HttpResponse::codes[response.getStatusCode()].explanation, HttpResponse::codes[response.getStatusCode()].explanation.size());

	response.addHeader("Content-Type", "text/plain");
	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());

	response.setMessageBody(body);

	delete cgi;
	return response;
}

HttpMethod *HttpPost::clone() {
	return new HttpPost();
}

std::string HttpPost::getName() {
	return "POST";
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpPost &HttpPost::operator=(HttpPost const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
