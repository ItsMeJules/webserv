#include "utils.hpp"

int ws::stoi(std::string const &str) {
	int nbr;
	std::stringstream(str) >> nbr;
	return nbr;
}

unsigned int ws::hextoi(std::string const &str) {
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return x;
}

std::string ws::itos(int const &nbr) {
	std::stringstream ss;
	ss << nbr;
	return ss.str();
}

bool ws::string_in_range(std::string const &range, std::string const &str) {
    for (int i = 0; str[i]; i++) {
        if (range.find(str[i]) == std::string::npos)
            return false;
    }
	return true;
}

std::string	int_to_string(int i)
{
	std::stringstream ss;
	ss << i;
	std::string s = ss.str();
	return s;
}
