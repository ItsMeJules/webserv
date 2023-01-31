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

	DefaultBody *body = new DefaultBody();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request, getName());
	std::ifstream fileStream;
	struct stat fileInfo;
	Cgi *cgi = new Cgi(serverInfo.getCgis());

	std::cout << "entre dans les cgi" << std::endl;

	if (serverInfo.getCgis().count(data.fileExtension) == 1) {
		std::string responseReturn = cgi->execute(request, data);
		if (responseReturn != "error")
			cgi->execute(request, data);
		else
			std::cout << "error" << std::endl;
		std::cout << "passe le execute" << std::endl;
	}
	else if (data.fileExtension == ".css")
		response.addHeader("Content-Type", "text/css");
	else if (data.fileExtension == ".html")
		response.addHeader("Content-Type", "text/html");
	else if (data.fileExtension == ".ico")
		response.addHeader("Content-Type", "text/favicon");

	std::cout << "passe les headers" << std::endl;

	stat(data.requestedPath.c_str(), &fileInfo);
	fileStream.open(data.requestedPath.c_str(), std::fstream::ate);

	if (!fileStream.is_open() || !S_ISREG(fileInfo.st_mode)) { // https://stackoverflow.com/a/40163393/10885193
		response.setStatusCode(404);
		body->append(HttpResponse::codes[404].explanation, HttpResponse::codes[404].explanation.size());
		response.addHeader("Content-Type", "text/plain");
		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Connection", "close");
		response.addHeader("Date", response.generateDate());

		std::cout << "random" << std::endl;

		fileStream.close();
		response.setMessageBody(body);
		return response;
	}

	data.fileSize = fileStream.tellg();
	fileStream.seekg(0);

	std::string fileContent = std::string(data.fileSize, ' ');
	fileStream.read(&fileContent[0], data.fileSize);

	body->append(fileContent, data.fileSize);
	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());

	response.setMessageBody(body);

	delete cgi;
	fileStream.close();
	return response;
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}





