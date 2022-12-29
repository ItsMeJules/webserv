#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>

namespace ws {
	// CONVERSIONS
	int stoi(std::string str);
	unsigned int hextoi(std::string str);
	std::string itos(int nbr);

	//I DONT KNOW
	bool char_in_string(char *tab, char);
}

#endif