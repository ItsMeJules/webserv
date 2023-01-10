#include <cstdint>
#include "ServerInfo.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

ServerInfo::ServerInfo() {}
ServerInfo::ServerInfo(ServerInfo const &serverInfo) { *this = serverInfo; }
ServerInfo::~ServerInfo() {}

ServerInfo::Location::Location() {}
ServerInfo::Location::Location(Location const &location) { *this = location; }
ServerInfo::Location::~Location() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

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

const std::map<std::string, std::string> &ServerInfo::getCgis() const {
    return _cgis;
}

const std::map<std::string, ServerInfo::Location> &ServerInfo::getLocations() const {
    return _locations;
}

const std::string &ServerInfo::Location::getIndexPath() const {
    return _indexPath;
}

bool ServerInfo::Location::hasAutoindex() const {
    return _autoindex;
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