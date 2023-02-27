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

const std::vector<std::string> &Location::getMethods() const {
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