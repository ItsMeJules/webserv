#include "ServerInfo.hpp"
#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ServerInfo::ServerInfo() {}
ServerInfo::ServerInfo(ServerInfo const &serverInfo) { *this = serverInfo; }
ServerInfo::~ServerInfo() {
    for (std::map<std::string, Location *>::iterator it = _locations.begin(); it != _locations.end(); it++) {
		if (it->second)
			delete it->second;
	}
}

// Location::Location() {}
// Location::Location(Location const &location) { *this = location; }
// Location::~Location() {}

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

const std::vector<std::string> &ServerInfo::getMethod() const {
	return _method;
}

const std::map<std::string, std::string> &ServerInfo::getCgis() const {
    return _cgis;
}

const std::map<std::string, Location *> &ServerInfo::getLocations() const {
    return _locations;
}

const std::map<int, std::string> &ServerInfo::getError() const {
	return _errorPage;
}


// const std::string &Location::getIndexPath() const {
//     return _indexPath;
// }

// bool Location::hasAutoindex() const {
//     return _autoindex;
// }

void  ServerInfo::setMaxBodySize(uint32_t MaxBodySize) {
	this->_maxBodySize = MaxBodySize;
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

void  ServerInfo::addtoMethod(std::string Method) {
	this->_method.push_back(Method);
}

void  ServerInfo::addToCGIS(std::string Extension, std::string Path) {
	this->_cgis[Extension] = Path;
}

void  ServerInfo::addErrorPage(int Key, std::string Value) {
	this->_errorPage[Key] = Value;
}

// ############## OPERATORS ##############

ServerInfo &ServerInfo::operator=(ServerInfo const &rhs) {
	if (this != &rhs) {
		_maxBodySize = rhs._maxBodySize;
		_autoindex = rhs._autoindex;
		_serverName = rhs._serverName;
		_rootPath = rhs._rootPath;
		_cgis = rhs._cgis;
		for (std::map<std::string, Location *>::const_iterator it = rhs._locations.begin(); it != rhs._locations.end(); it++) {
			
		}
		
	}

	return *this;
}

// ServerInfo::Location &ServerInfo::Location::operator=(ServerInfo::Location const &rhs) {
// 	if (this != &rhs) {
// 		_indexPath = rhs._indexPath;
// 		_autoindex = rhs._autoindex;
// 	}
// 	return *this;
// }