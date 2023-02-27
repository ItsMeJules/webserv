#include "Location.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Location::Location() : _autoIndex(-1) {}
Location::Location(Location const &location) { *this = location; }
Location::~Location() {}

// ############## GETTERS / SETTERS ##############

const std::string &Location::getIndexPath() const {
	return _indexPath;
}

const std::string &Location::getRootPath() const {
	return _rootPath;
}

const std::string &Location::getUploadPath() const {
	return _uploadPath;
}

const int &Location::getAutoindex() const {
	return _autoIndex;
}

const std::vector<std::string> &Location::getMethod() const {
	return _method;
}

void  Location::setIndexPath(std::string Path) {
	this->_indexPath = Path;
}

void  Location::setRootPath(std::string Path) {
	this->_rootPath = Path;
}

void  Location::setUploadPath(std::string Path) {
	this->_uploadPath = Path;
}

void  Location::setAutoIndex(int AutoIndex) {
	this->_autoIndex = AutoIndex;
}

void  Location::addtoMethod(std::string Method) {
	this->_method.push_back(Method);
}

// ############## OPERATORS ##############

Location &Location::operator=(Location const &rhs) {
	if (this != &rhs) {
		_indexPath = rhs._indexPath;
		_rootPath = rhs._rootPath;
		_uploadPath = rhs._uploadPath;
		_autoIndex = rhs._autoIndex;
		_method = rhs._method;
	}
	return *this;
}

 # include <iostream>

const Location *Location::getBestMatch(std::string const &url, std::map<std::string, Location*> const &map) {
	Location *location = _defaultLocation;
	size_t maxSize = 0;

	for (std::map<std::string, Location*>::const_iterator it = map.begin(); it != map.end(); it++) {
		std::string path = it->first;

		if (url.substr(0, path.size()) == path) {
			if ((url.size() > path.size() && url[path.size()] == '/') || url.size() == path.size()) {
				if (path.size() > maxSize) {
					maxSize = path.size();
					location = it->second;
				}
			}
		}
	}
	return location;
}
