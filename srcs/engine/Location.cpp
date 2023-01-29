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

const std::string &Location::getRewritePath() const {
	return _rewritePath;
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

void  Location::setRewritePath(std::string Path) {
	this->_rewritePath = Path;
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
		_autoIndex = rhs._autoIndex;
	}
	return *this;
}