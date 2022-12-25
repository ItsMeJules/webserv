#include "converter.hpp"

int ws::stoi(std::string str) {
	int nbr;
	std::stringstream(str) >> nbr;
	return nbr;
}

unsigned int ws::hextoi(std::string str) {
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return x;
}

std::string ws::itos(int nbr) {
	std::stringstream ss;
	ss << nbr;
	return ss.str();	
}