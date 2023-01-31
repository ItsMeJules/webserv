#include "HttpGet.hpp"

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


HttpResponse HttpGet::execute(ServerInfo const &info, HttpRequest &request) {
	HttpResponse response;
	DefaultBody *body = new DefaultBody();
	std::ifstream file;
	struct stat fileInfo;
	std::string requestedPath = request.getPath();
	_isCgi = true;

	if (request.getPath()[0] == '/')
		request.setPath(info.getRootPath() + request.getPath());

	if (request.getPath() == info.getRootPath() + "/")
		request.setPath(info.getIndexPath());

	stat(request.getPath().c_str(), &fileInfo);
	file.open(request.getPath().c_str(), std::fstream::ate);

	if (!file.is_open() || !S_ISREG(fileInfo.st_mode)) { // https://stackoverflow.com/a/40163393/10885193
		response.setStatusCode(404);
		std::cout << "REQUEST HAS FAILED" << std::endl;
		body->append(HttpResponse::codes[404].explanation, HttpResponse::codes[404].explanation.size());
		response.addHeader("Content-Type", "text/plain");
		response.addHeader("Content-Length", ws::itos(body->getBody().size() - 1));
		response.addHeader("Connection", "close");
		response.generateDate();

		file.close();
		response.setMessageBody(body);
		return response;
	}

	if (request.getPath() == info.getIndexPath())
		response.addHeader("Content-Type", "text/html");
	else if (request.getPath() == "index.css")
		response.addHeader("Content-Type", "text/css");
	if (request.getPath() == "favicon.ico")
		response.addHeader("Content-Type", "text/favicon");

	size_t fileSize = file.tellg();
	std::string fileContent = std::string(fileSize, ' ');

	file.seekg(0);
	file.read(&fileContent[0], fileSize);

	body->append(fileContent, fileSize);
	response.addHeader("Content-Length", ws::itos(body->getBody().size() - 1));

	response.setMessageBody(body);
	return response;
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}





