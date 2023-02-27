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
	Location *location;
	size_t maxSize = 0;

	for (std::map<std::string, Location*>::const_iterator it = map.begin(); it != map.end(); it++) {
		std::string path = it->first;

		std::cout << it->second->getIndexPath() << std::endl;
		std::cout << it->second->getRootPath() << std::endl;

		std::cout << url << " | " << path << " || " << std::endl;
		std::cout << url.substr(0, path.size()) << " | " << path << std::endl << std::endl;
		if (url.substr(0, path.size()) == path) {
			std::cout << "found!" << std::endl;
			if ((url.size() > path.size() && url[path.size()] == '/') || url.size() == path.size()) {
				std::cout << "found with extra uri" << std::endl;
				if (path.size() > maxSize) {
					std::cout << "better match found!" << std::endl << std::endl;
					maxSize = path.size();
					location = it->second;
				}
			}
		}
	}
	// std::cout << location->getRootPath() << std::endl;
	return location;
}