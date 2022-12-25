#ifndef CONVERTER_HPP
# define CONVERTER_HPP

# include <string>
# include <sstream>

namespace ws {
	int stoi(std::string str);
	unsigned int hextoi(std::string str);
	std::string itos(int nbr);
}

#endif