#include "Server.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Server::Server() : _socket() {}
Server::Server(ServerSocket &socket) : _socket(socket) {}
Server::Server(Server const &server) { *this = server; }
Server::~Server() {
	ws::log(ws::LOG_LVL_INFO, "[SERVER] -", "closing server");
}

// ############## PRIVATE ##############

bool Server::startListening(int backlog) {
	int ret = listen(_socket.getFd(), backlog);
	if (ret == -1)
		ws::log(ws::LOG_LVL_ERROR, "[SERVER] -", "failed to listen on fd: " + ws::itos(_socket.getFd()) + "!", true);
	else
		ws::log(ws::LOG_LVL_SUCCESS, "[SERVER] -", "successfully listening on fd: " + ws::itos(_socket.getFd()));
	return !ret;
}

// ############## PUBLIC ##############

bool Server::setup() {
	ws::log(ws::LOG_LVL_INFO, "[SERVER] -", "setting up the server...");
    return startListening(10) && poller->init() && poller->pollFd(_socket.getFd(), poller->pollInEvent());
}

bool Server::receiveData(Client &client) {
	char buffer[ws::RECV_BUFFER_SIZE + 1];
	memset(buffer, 0, ws::RECV_BUFFER_SIZE);

	int clientFd = client.getSocket().getFd();
	int byteCount = recv(clientFd, buffer, ws::RECV_BUFFER_SIZE, 0);
	if (byteCount > 0) {
        buffer[byteCount] = 0;
		ws::log(ws::LOG_LVL_INFO, "[SERVER] -", ws::itos(byteCount) + " bytes were read from fd: " + ws::itos(clientFd));
		ws::log(ws::LOG_LVL_DEBUG, "", std::string("content:\n") + buffer);
        client.getRequestParser().parseRequest(buffer);
    } else if (byteCount == 0) {
		ws::log(ws::LOG_LVL_INFO, "[SERVER] -", "0 byte was read from fd: " + ws::itos(clientFd));
        return false;
	} else
		ws::log(ws::LOG_LVL_ERROR, "[SERVER] -", "recv returned an error with fd " + ws::itos(clientFd), true);
    return true;
}

void Server::sendData(Client &client, HttpResponse &response) {
    response.send(client);
}

bool Server::connect(Client &client) {
    _clients.insert(std::make_pair(client.getSocket().getFd(), client));
    return poller->pollFd(client.getSocket().getFd(), poller->pollInEvent());
}

bool Server::disconnect(Client &client) {
	ws::log(ws::LOG_LVL_INFO, "[SERVER] -", "disconnecting client...");
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

const ServerSocket &Server::getSocket() const {
	return _socket;
}

ServerInfo &Server::getServerInfo() {
    return _serverInfo;
}

// ############## OPERATORS ##############

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		_socket = rhs._socket;
		_clients = rhs._clients;
	}
	return *this;
}
