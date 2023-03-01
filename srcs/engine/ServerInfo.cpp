#include "ServerInfo.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ServerInfo::ServerInfo() { // TODO changer ca
	_maxBodySize = 1000000;
	_autoindex = false;
	_serverName = "";
	_rootPath = "html";
	_indexPath = "index.html";
	_ip = "0.0.0.0";
	_port = 8000;
}
ServerInfo::ServerInfo(ServerInfo const &serverInfo) {
	*this = serverInfo;
}
ServerInfo::~ServerInfo() {
    for (std::map<std::string, Location *>::iterator it = _locations.begin(); it != _locations.end(); it++) {
		if (it->second)
			delete it->second;
	}
}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void ServerInfo::addLocation(std::string path, Location *location) {
    _locations.insert(std::make_pair(path, location));
}

// ############## GETTERS / SETTERS ##############

const uint32_t &ServerInfo::getMaxBodySize() const {
    return _maxBodySize;
}

bool ServerInfo::hasAutoindex() const {
    return _autoindex;
}

const std::string &ServerInfo::getServerName() const {
    return _serverName;
}

const std::string &ServerInfo::getRootPath() const {
    return _rootPath;
}

const std::string &ServerInfo::getIndexPath() const {
	return _indexPath;
}

const std::string &ServerInfo::getUploadPath() const {
	return _uploadPath;
}

const std::string &ServerInfo::getIp() const {
	return _ip;
}

const int &ServerInfo::getPort() const {
	return _port;
}

const std::vector<std::string> &ServerInfo::getMethod() const {
	return _method;
}

const std::map<std::string, std::string> &ServerInfo::getCgis() const {
    return _cgis;
}

const std::map<std::string, Location *> &ServerInfo::getLocations() const {
    return _locations;
}

std::map<int, std::string> &ServerInfo::getError() {
	return _errorPage;
}

const std::map<int, std::string> &ServerInfo::getErrorPages() const {
	return _errorPage;
}

const Location &ServerInfo::getDefaultLocation() const {
	return _defaultLocation;
}

void  ServerInfo::setMaxBodySize(uint32_t MaxBodySize) {
	this->_maxBodySize = MaxBodySize;
}

void  ServerInfo::setIp(std::string ip) {
	this->_ip = ip;
}

void  ServerInfo::setPort(int port) {
	this->_port = port;
}

void  ServerInfo::setAutoIndex(bool AutoIndex) {
	this->_autoindex = AutoIndex;
}

void  ServerInfo::setServerName(std::string ServerName) {
	this->_serverName = ServerName;
}

void  ServerInfo::setRootPath(std::string Path) {
	this->_rootPath = Path;
}

void  ServerInfo::setIndexPath(std::string Path) {
	this->_indexPath = Path;
}

void  ServerInfo::setUploadPath(std::string Path) {
	this->_uploadPath = Path;
}

void  ServerInfo::addtoMethod(std::string method) {
	this->_method.push_back(method);
}

void  ServerInfo::addToCGIS(std::string Extension, std::string Path) {
	this->_cgis[Extension] = Path;
}

void  ServerInfo::addErrorPage(int Key, std::string Value) {
	this->_errorPage[Key] = Value;
}

void ServerInfo::setLocation(std::map<std::string, Location*> locations) {
	_locations = locations;
}

void ServerInfo::setErrorPage(std::map<int, std::string> error) {
	_errorPage = error;
}

void ServerInfo::setDefaultLocation(Location defaultLocation) {
	_defaultLocation = defaultLocation;
}


// ############## OPERATORS ##############

ServerInfo &ServerInfo::operator=(ServerInfo const &rhs) {
	if (this != &rhs) {
		_maxBodySize = rhs._maxBodySize;
		_autoindex = rhs._autoindex;
		_serverName = rhs._serverName;
		_rootPath = rhs._rootPath;
		_cgis = rhs._cgis;
		_uploadPath = rhs._uploadPath;
		_method = rhs._method;
		_cgis = rhs._cgis;
		_errorPage = rhs._errorPage;
		_defaultLocation = rhs._defaultLocation;

		for (std::map<std::string, Location *>::const_iterator it = rhs._locations.begin(); it != rhs._locations.end(); it++)
			_locations.insert(std::make_pair(it->first, new Location(*it->second)));
	}

	return *this;
}
