#include "Server.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Server::Server() : _socket(), _poller(NULL) {
}

Server::Server(ServerSocket &socket, IPoll *poller) : _socket(socket), _poller(poller) {
	startListening(10);
	poller->init();
	poller->pollFd(socket.getFd(), poller->listenerEvents());
}

Server::Server(Server const &server) { *this = server; }
Server::~Server() {}

// ############## PRIVATE ##############

bool Server::startListening(int backlog) {
	int ret = listen(_socket.getFd(), backlog);
	if (ret == -1)
		std::cerr << "failed to listen on fd: " << _socket.getFd() << " " << strerror(errno) << std::endl;
	else
		std::cout << "successfully listening on fd: " << _socket.getFd() << std::endl;
	return ret;
}


// ############## PUBLIC ##############

void Server::receiveData(Client &client) {
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, 0, BUFFER_SIZE);

	int clientFd = client.getSocket().getFd();
	int byteCount = recv(clientFd, buffer, BUFFER_SIZE, 0);
	if (byteCount > 0) {
        buffer[byteCount] = 0;
        client.getRequestParser().parseRequest(buffer);
    } else if (byteCount == 0)
        disconnect(client); // if header Connection: keep-alive don't close
	else
		std::cout << "recv returned an error with fd " << clientFd << ": " << strerror(errno) << std::endl;
}

void Server::sendData(Client &client, HttpResponse &response) {
    response.send(client);
}

bool Server::connect(Client &client) {
    _clients.insert(std::make_pair(client.getSocket().getFd(), client));
    return _poller->pollFd(client.getSocket().getFd(), _poller->clientEvents());
}

bool Server::disconnect(Client &client) {
    bool ret = client.getSocket().close(_poller);
    _clients.erase(client.getSocket().getFd());
    return ret;
}

Client &Server::getClient(int fd) {
	return _clients[fd];
}

// ############## GETTERS / SETTERS ##############

ServerSocket Server::getSocket() {
	return _socket;
}

IPoll *Server::getPoller() {
	return _poller;
}

// ############## OPERATORS ##############

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		_socket = rhs._socket;
		_poller = rhs._poller;
		_clients = rhs._clients;
	}
	return *this;
}
