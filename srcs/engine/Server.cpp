#include "Server.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Server::Server() : _socket() {}
Server::Server(ServerSocket &socket) : _socket(socket) {}
Server::Server(Server const &server) { *this = server; }
Server::~Server() {}

// ############## PRIVATE ##############

bool Server::startListening(int backlog) {
	int ret = listen(_socket.getFd(), backlog);
	if (ret == -1)
		std::cerr << "failed to listen on fd: " << _socket.getFd() << " " << strerror(errno) << std::endl;
	else
		std::cout << "successfully listening on fd: " << _socket.getFd() << std::endl;
	return !ret;
}

// ############## PUBLIC ##############

bool Server::setup() {
    return startListening(10) && poller->init() && poller->pollFd(_socket.getFd(), poller->listenerEvents());
}

bool Server::receiveData(Client &client) {
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, 0, BUFFER_SIZE);

	int clientFd = client.getSocket().getFd();
	int byteCount = recv(clientFd, buffer, BUFFER_SIZE, 0);
	if (byteCount > 0) {
        buffer[byteCount] = 0;
        client.getRequestParser().parseRequest(buffer);
    } else if (byteCount == 0)
        return false;
	else
		std::cout << "recv returned an error with fd " << clientFd << ": " << strerror(errno) << std::endl;
    return true;
}

void Server::sendData(Client &client, HttpResponse &response) {
    response.send(client);
}

bool Server::connect(Client &client) {
    _clients.insert(std::make_pair(client.getSocket().getFd(), client));
    return poller->pollFd(client.getSocket().getFd(), poller->clientEvents());
}

bool Server::disconnect(Client &client) {
    bool ret = client.getSocket().close();
    _clients.erase(client.getSocket().getFd());
    return ret;
}

Client &Server::getClient(int fd) {
	return _clients[fd];
}

// ############## GETTERS / SETTERS ##############

const ServerSocket &Server::getSocket() const {
	return _socket;
}

ServerInfo &Server::getServerInfo() {
    return _serverInfo;
}

std::string Server::getName() const
{
	return _name;
}

// ############## OPERATORS ##############

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		_socket = rhs._socket;
		_clients = rhs._clients;
	}
	return *this;
}
