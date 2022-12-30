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
	bool string_in_range(std::string const &range, std::string const &str);
}

#endif