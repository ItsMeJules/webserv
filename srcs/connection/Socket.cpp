# include "ASocket.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ASocket::ASocket() {}
ASocket::ASocket(ASocket const &socket) { *this = socket; }
ASocket::~ASocket() {}

// ############## PUBLIC ##############

bool ASocket::close() const {
	bool ret = ::close(_fd) == 0;
	if (ret)
		ws::log(ws::LOG_LVL_SUCCESS, "[ASOCKET] -", "successfully closed socket with fd: " + ws::itos(_fd));
	else
		ws::log(ws::LOG_LVL_ERROR, "[ASOCKET] -", "failed to close socket with fd: "+ ws::itos(_fd) + "!", true);
    return ret;
}

// ############## GETTERS / SETTERS ##############
const int &ASocket::getFd() const {
	return _fd;
}

const sockaddr_in &ASocket::getAddress() const {
	return _address;
}

// ############## OPERATORS ##############

ASocket &ASocket::operator=(ASocket const &rhs) {
	if (this != &rhs) {
		_fd = rhs._fd;
		_address = rhs._address;
	}
	return *this;
}