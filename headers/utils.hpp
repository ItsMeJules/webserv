#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <vector>


namespace ws {
	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

	//I DONT KNOW
	bool string_in_range(std::string const &range, std::string const &str);
	std::string					ft_to_upper(std::string &str);


	std::vector<std::string> ft_split(const std::string &str, const std::string &charset);
	std::string					replace(std::string src, std::string search, std::string replace);

}

#endif
