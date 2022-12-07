# include "Socket.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Socket::Socket() :
		type_(SOCK_STREAM), protocol_(0), binded_(false) {}

Socket::Socket(int domain, int type, int protocol) : Connection(domain), 
		type_(SOCK_STREAM), protocol_(protocol), binded_(false) {}

Socket::Socket(Socket const &socket) { *this = socket; }

Socket::~Socket() {}

// ############## PUBLIC ##############

bool Socket::generateFd() {
	this->fd_ = socket(this->domain_, this->type_, this->protocol_);
	if (this->fd_ == -1)
		std::cerr << "error while creating a socket:\n" << strerror(errno) << std::endl;
	else
		std::cout << "sucessfully generated fd: " << this->fd_ << std::endl;
	return this->fd_ != -1;
}

bool Socket::bindTo(unsigned int port) {
	this->setupAddress();
	this->address_.sin_port = htons(port);

	binded_ = bind(this->fd_, (struct sockaddr *)&this->address_, sizeof(this->address_)) == 0;
	if (binded_)
		std::cout << "sucessfully binded fd: " << this->fd_ << std::endl;
	else
		std::cerr << "error while binding fd: " << this->fd_ << "\nerror:" << strerror(errno) << std::endl;
	return binded_;
}

bool Socket::setNonBlocking() {
	int flags = fcntl(this->fd_, F_GETFL, 0);
	if (flags == -1)
		flags = 0;
	return fcntl(this->fd_, F_SETFL, flags | O_NONBLOCK);
}

bool Socket::setReusable() {
	const int enable = 1;
	return setsockopt(this->fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0;
}

// ############## GETTERS / SETTERS ##############

int Socket::getType() const {
	return this->type_;
}

int Socket::getProtocol() const {
	return this->protocol_;
}

int Socket::getBacklog() const {
	return this->backlog_;
}

bool Socket::isBinded() const {
	return this->binded_;
}

// ############## OPERATORS ##############

Socket &Socket::operator=(Socket const &rhs) {
	if (this != &rhs) {
		this->domain_ = rhs.domain_;
		this->fd_ = rhs.fd_;
		this->type_ = rhs.type_;
		this->protocol_ = rhs.protocol_;
		this->fd_ = rhs.fd_;
		this->backlog_ = rhs.backlog_;
		this->address_ = rhs.address_;
	}
	return *this;
}