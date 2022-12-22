#include "converter.hpp"

int ws::stoi(std::string str) {
	int nbr;
	std::stringstream(str) >> nbr;
	return nbr;
}

std::string ws::itos(int nbr) {
	std::stringstream ss;
	ss << nbr;
	return ss.str();	
}	