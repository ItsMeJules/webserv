#include "HttpGet.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpGet::HttpGet() {}
HttpGet::HttpGet(HttpGet const &httpGet) { *this = httpGet; }
HttpGet::~HttpGet() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############


HttpResponse HttpGet::execute(ServerInfo const &info, HttpRequest &request) {
	HttpResponse response;
	DefaultBody *body = new DefaultBody();
	std::ifstream file;

	if (request.getPath() == "/" || request.getPath() == info.getIndexPath()) {
		file.open(info.getIndexPath().c_str(), std::fstream::ate);

		std::cout << file.is_open() << std::endl;

		size_t fileSize = file.tellg();
		std::string fileContent = std::string(fileSize, ' ');
		file.seekg(0);
		file.read(&fileContent[0], fileSize);

		body->append(fileContent, fileSize);
		response.addHeader("Content-Type", "text/html");
		response.addHeader("Content-Length", ws::itos(body->getBody().size() - 1));
	}

	std::cout << " hehefh " << std::endl;
	response.setMessageBody(body);
	file.close();
	return response;
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpGet &HttpGet::operator=(HttpGet const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}
