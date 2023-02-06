#include "Location.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Location::Location() : _autoIndex(false) {}
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

const bool &Location::hasAutoindex() const {
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

void  Location::setAutoIndex(bool AutoIndex) {
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

const Location *Location::getBestMatch(std::string const &str, std::map<std::string, Location*> const &map) {
	Location *location;

	for (std::map<std::string, Location*>::const_iterator it = map.begin(); it != map.end(); it++) {
		std::string path = it->first;

		if (path.size() == str.size()) {
			
		} else if (path.size() > str.size()) {
			
		}

		location = it->second;
		break ;
	}
	return location;
}