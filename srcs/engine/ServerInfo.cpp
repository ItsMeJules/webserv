#include "ServerInfo.hpp"
#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ServerInfo::ServerInfo() {}
ServerInfo::ServerInfo(ServerInfo const &serverInfo) { *this = serverInfo; }
ServerInfo::~ServerInfo() {
    for (std::map<std::string, ServerInfo::Location *>::iterator it = _locations.begin(); it != _locations.end(); it++)
        delete it->second;
}

ServerInfo::Location::Location() {}
ServerInfo::Location::Location(Location const &location) { *this = location; }
ServerInfo::Location::~Location() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void ServerInfo::addLocation(std::string path, ServerInfo::Location *location) {
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

const std::vector<std::string> &ServerInfo::getMethod() const {
	return _method;
}

const std::map<std::string, std::string> &ServerInfo::getCgis() const {
    return _cgis;
}

const std::map<std::string, ServerInfo::Location*> &ServerInfo::getLocations() const {
    return _locations;
}

const std::string &ServerInfo::Location::getIndexPath() const {
    return _indexPath;
}

bool ServerInfo::Location::hasAutoindex() const {
    return _autoindex;
}

void  ServerInfo::setMaxBodySize(uint32_t MaxBodySize) {
	this->_maxBodySize = MaxBodySize;
}

void  ServerInfo::setAutoIndex(bool AutoIndex) {
	this->_autoindex = AutoIndex;
}

void  ServerInfo::setServerName(std::string ServerName) {
	this->_serverName = ServerName;
}

void  ServerInfo::setIndexPath(std::string path) {
	this->_indexPath = path;
}

void  ServerInfo::addtoMethod(std::string method) {
	this->_method.push_back(method);
}

void  ServerInfo::addToCGIS(std::string extension, std::string path) {
	this->_cgis[extension] = path;
}

// ############## OPERATORS ##############

ServerInfo &ServerInfo::operator=(ServerInfo const &rhs) {
	if (this != &rhs) {
		_maxBodySize = rhs._maxBodySize;
		_autoindex = rhs._autoindex;
		_serverName = rhs._serverName;
		_rootPath = rhs._rootPath;
		_cgis = rhs._cgis;
		_locations = rhs._locations;
	}
	return *this;
}

ServerInfo::Location &ServerInfo::Location::operator=(ServerInfo::Location const &rhs) {
	if (this != &rhs) {
		_indexPath = rhs._indexPath;
		_autoindex = rhs._autoindex;
	}
	return *this;
}