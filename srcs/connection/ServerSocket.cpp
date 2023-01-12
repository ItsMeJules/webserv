# include "ServerSocket.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ServerSocket::ServerSocket() : _domain(-1), _port(-1) {
    setup();
}

ServerSocket::ServerSocket(int domain, int type, int protocol, int port)
	: _domain(domain), _type(type), _protocol(protocol), _port(port)
{
    setup();
}

ServerSocket::ServerSocket(std::string ip, int port) : _ip(ip), _port(port) {

}

ServerSocket::ServerSocket(ServerSocket const &socket) { *this = socket; }
ServerSocket::~ServerSocket() {}

// ############## PUBLIC ##############

// ############## PRIVATE ##############

bool ServerSocket::setup() {
    if (_domain == -1) {
        _domain = AF_INET;
        _type = SOCK_STREAM;
        _protocol = 0;
        if (_port == -1)
            _port = 9999;
    }
    return generateFd() && setReusable() && setNonBlocking() && bindTo();
}

bool ServerSocket::generateFd() {
	_fd = socket(_domain, _type, _protocol);
	if (_fd == -1)
		std::cerr << "error while creating a socket:\n" << strerror(errno) << std::endl;
	else
		std::cout << "sucessfully generated fd: " << _fd << std::endl;
	return _fd != -1;
}

bool ServerSocket::setReusable() {
	const int enable = 1;
	int ret = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	if (ret != -1) {
		std::cout << "sucessfully set socket on fd: " << _fd << " reusable" << std::endl;
		return true;
	}
	std::cerr << "error while binding fd: " << _fd << "\nerror:" << strerror(errno) << std::endl;
	return false;
}

bool ServerSocket::setNonBlocking() {
	int flags = fcntl(_fd, F_GETFL, 0);
	if (flags == -1)
		flags = 0;
	int ret = fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
	if (ret != -1) {
		std::cout << "sucessfully set socket non blocking on fd: " << _fd << std::endl;
		return true;
	}
	std::cerr << "error while setting socket non blocking on fd: " << _fd << "\nerror:" << strerror(errno) << std::endl;
	return false;
}

bool ServerSocket::bindTo() {
	memset((char *)&_address, 0, sizeof(struct sockaddr_in));
	_address.sin_family = _domain;
	_address.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());
	_address.sin_port = htons(_port);

	bool binded = bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) == 0;
	if (binded)
		std::cout << "sucessfully binded fd: " << _fd << std::endl;
	else
		std::cerr << "error while binding fd: " << _fd << "\nerror:" << strerror(errno) << std::endl;
	return binded;
}

// ############## GETTERS / SETTERS ##############

std::string const &ServerSocket::getIp() const {
    return _ip;
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
		Socket::operator=(rhs);
	}
	return *this;
}