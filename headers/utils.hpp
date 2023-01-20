#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <vector>

# include <algorithm>

namespace ws {
    const std::string WHITE_SPACES = " \t\n\v\f\r";
    const std::string LOW_LETTERS = "abcdefghijklmnopqrstuvwxyz";
    const std::string UP_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string LETTERS = LOW_LETTERS + UP_LETTERS;

	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

	//I DONT KNOW
	std::string					ft_to_upper(std::string &str);



    // STRING MANIPULATION
    std::string &skip_chars(std::string &str, std::string const &toSkip);
	bool string_in_range(std::string const &range, std::string const &str, size_t npos = std::string::npos);
	void	ft_strcpy(const char *src, char *dest);

}
	std::string					replace(std::string src, std::string search, std::string replace);
	std::vector<std::string> ft_split(const std::string &str, const std::string &charset);
	std::string	ft_to_upper(std::string &str);
	void	*ft_memset(void *b, int c, size_t len);
	void	ft_strcpy(const char *src, char *dest);




#endif
