# include "ServerSocket.hpp"
# include "Server.hpp"
# include "EPoll.hpp"
# include "RequestParser.hpp"
# include "StatusCode.hpp"

int main() {
	// RequestParser request_parser;
	// std::string	header;
	StatusCode status_code;
	ServerSocket serverSocket;
	serverSocket.setup();
	std::string request = "Some Request";

	IPoll *poller = new EPoll();
	Server server(serverSocket, poller);

	std::cout << "HELLO WORLD" << std::endl;
	status_code.recupInfoParserRequest(request);
	std::cout << "StatusCode: " << status_code.getStatusCode() << std::endl;
	while (poller->polling(server) > 0) {
	}
    serverSocket.close(poller);
	// request_parser.returnParse(header);
	delete poller;
}
