#include "AMessageBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

AMessageBody::AMessageBody(ADataDecoder *decoder) : _decoder(decoder) {
	_tmpFile.name = ws::TMP_PATH + "/tmp_file";

	ws::make_tmp_file(_tmpFile);
	_tmpOfStream.open(_tmpFile.name.c_str(), std::ofstream::out);
}

AMessageBody::AMessageBody() : _decoder(NULL) {
}

AMessageBody::AMessageBody(AMessageBody const &messageBody) { 
	*this = messageBody;
}

AMessageBody::~AMessageBody() {
	destroyTmpFile();
	if (_decoder != NULL)
		delete _decoder;
}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

bool AMessageBody::writeToFile(std::vector<char> const &vec) {
	_tmpOfStream.write(vec.data(), vec.size());
	return true;
}

bool AMessageBody::appendFromFile(std::vector<char> &vec, std::vector<char>::size_type readSize) {
	char buffer[readSize];

	if (vec.size() < readSize)
		vec.reserve(vec.size() + readSize);

	_tmpIfStream.read(buffer, readSize);

	vec.insert(vec.end(), &buffer[0], &buffer[_tmpIfStream.gcount()]);
	return true;
}

void AMessageBody::destroyTmpFile() {
	_tmpOfStream.close();
	_tmpIfStream.close();
	unlink(_tmpFile.name.c_str());
}

// ############## GETTERS / SETTERS ##############

ADataDecoder *AMessageBody::getDataDecoder() {
	return _decoder;
}

void AMessageBody::setDataDecoder(ADataDecoder *decoder) {
	_decoder = decoder;
}

// ############## OPERATORS ##############

AMessageBody &AMessageBody::operator=(AMessageBody const &rhs) {
	if (this != &rhs) {
		_decoder = (rhs._decoder != NULL) ? rhs._decoder->clone() : NULL;
		_tmpFile = rhs._tmpFile;
	}
	return *this;
}
