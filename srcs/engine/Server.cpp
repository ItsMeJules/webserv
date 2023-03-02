#include "Server.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Server::Server() : _socket() {}
Server::Server(ServerSocket &socket) : _socket(socket) {}
Server::Server(Server const &server) { *this = server; }
Server::~Server() {
	delete poller;
}

// ############## PRIVATE ##############

bool Server::startListening(int backlog) {
	int ret = listen(_socket.getFd(), backlog);
	return !ret;
}

// ############## PUBLIC ##############

bool Server::setup() {
    return startListening(10) && poller->init() && poller->pollFd(_socket.getFd(), poller->pollInEvent());
}

bool Server::receiveData(Client &client) {
	char buffer[ws::RECV_BUFFER_SIZE + 1];
	memset(buffer, 0, ws::RECV_BUFFER_SIZE);

	int clientFd = client.getSocket().getFd();
	int byteCount = recv(clientFd, buffer, ws::RECV_BUFFER_SIZE, 0);
	if (byteCount > 0) {
        buffer[byteCount] = 0;
        if (!client.getRequestParser().parseRequest(buffer, byteCount, _serverInfo.getMaxBodySize())) {
			client.setRequestFailed(true);
			return true;
		}
    } else if (byteCount == 0) {
        return false;
	}
    return true;
}

void Server::sendData(Client &client, HttpResponse &response) {
	std::string responseStr = response.build();

	write(client.getSocket().getFd(), responseStr.c_str(), responseStr.length());
}

bool Server::connect(Client &client) {
    _clients.insert(std::make_pair(client.getSocket().getFd(), client));
    return poller->pollFd(client.getSocket().getFd(), poller->pollInEvent());
}

bool Server::disconnect(Client &client) {
    bool ret = poller->deleteFd(client.getSocket().getFd()) && client.getSocket().close();
    _clients.erase(client.getSocket().getFd());
    return ret;
}

bool Server::isConnected(int const fd) {
	return _clients.count(fd);
}

Client &Server::getClient(int const fd) {
	return _clients[fd];
}

// ############## GETTERS / SETTERS ##############

ServerSocket &Server::getServerSocket() {
	return _socket;
}

ServerInfo &Server::getServerInfo() {
    return _serverInfo;
}

void Server::setServerSocket(ServerSocket serverSocket) {
	_socket = serverSocket;
}

// ############## OPERATORS ##############

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		_socket = rhs._socket;
		_clients = rhs._clients;
	}
	return *this;
}
