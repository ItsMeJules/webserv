#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <algorithm>
# include <vector>

# include <string.h>
# include <errno.h>
# include <sys/stat.h>

# include "Constants.hpp"

namespace ws {

	// HTTP
	typedef struct http_status_s {
		std::string reason;
		std::string explanation;
	} http_status_t;

	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

    // STRING MANIPULATION
    std::string &skip_chars(std::string &str, std::string const &toSkip);
	bool string_in_range(std::string const &range, std::string const &str, size_t npos = std::string::npos);
	char *char_array(std::string const &str, int end, int begin = 0);
	int pos_in_vec(std::string const &str, std::vector<char> const &vec);
	int pos_in_vec_from_end(std::string const &str, std::vector<char> const &vec);

    // LOGGING
    void log(int const &level, std::string const &prefix, std::string const &message, const bool &_errno = false);

	char	**ft_split(char const *s, char c);
	static char	*dupword(char const *s, char c, int n);
	static int	wordlen(char const *s, char c);
	static int	words_count(char const *s, char c);
	static int	check_charset(char c, char current);
	// FILES
	bool file_exists(std::string const &path);
	bool file_is_reg(std::string const &path);
}

#endif
