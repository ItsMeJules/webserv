#include "Server.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Server::Server() : _socket(), _poller(NULL) {
	_clients = std::vector<Client>();
}

Server::Server(ServerSocket &socket, IPoll *poller) : _socket(socket), _poller(poller) {
	startListening(10);
	poller->init();
	poller->pollFd(socket.getFd(), poller->readEvent()); //peut etre d'autres event a mettre
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

void Server::addClient(Client client) {
	_clients.push_back(client);
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
