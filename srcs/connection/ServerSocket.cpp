# include "ServerSocket.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ServerSocket::ServerSocket(std::string ip, int port) : _type(SOCK_STREAM), _protocol(0), _port(port), _ip(ip) {}
ServerSocket::ServerSocket(int port) : _domain(AF_INET), _type(SOCK_STREAM), _port(port) {}

ServerSocket::ServerSocket() : _domain(AF_INET), _type(SOCK_STREAM), _protocol(0), _port(-1) {}

ServerSocket::ServerSocket(ServerSocket const &socket) : ASocket(*this) { *this = socket; }
ServerSocket::~ServerSocket() {}

// ############## PUBLIC ##############

// ############## PRIVATE ##############

bool ServerSocket::setup() {
    if (_port == -1) {
		_port = 9999;
	}

    return generateFd() && setReusable() && setNonBlocking() && bindTo();
}

bool ServerSocket::generateFd() {
	_fd = socket(_domain, _type, _protocol);
	return _fd != -1;
}

bool ServerSocket::setReusable() {
	const int enable = 1;
	int ret = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	return ret != -1;
}

bool ServerSocket::setNonBlocking() {
	int flags = fcntl(_fd, F_GETFL, 0);
	if (flags == -1)
		flags = 0;
	int ret = fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
	return ret != -1;
}

bool ServerSocket::bindTo() {
	memset((char *)&_address, 0, sizeof(struct sockaddr_in));
	_address.sin_family = _domain;
	_address.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());
	_address.sin_port = htons(_port);

	bool bound = bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) == 0;
	return bound;
}

// ############## GETTERS / SETTERS ##############

int const &ServerSocket::getPort() const {
	return _port;
}

std::string const &ServerSocket::getIp() const {
    return _ip;
}

void ServerSocket::setPort(int port) {
	_port = port;
}

void ServerSocket::setIp(std::string ip) {
    _ip = ip;
}

// ############## OPERATORS ##############

ServerSocket &ServerSocket::operator=(ServerSocket const &rhs) {
	if (this != &rhs) {
		_domain = rhs._domain;
		_type = rhs._type;
		_protocol = rhs._protocol;
		_port = rhs._port;
		ASocket::operator=(rhs);
	}
	return *this;
}