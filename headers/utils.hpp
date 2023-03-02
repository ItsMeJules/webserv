#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <algorithm>
# include <vector>
# include <fstream>

# include <string.h>
# include <errno.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>

# include "Constants.hpp"
# include "Location.hpp"

namespace ws {

	// HTTP
	typedef struct http_status_s {
		std::string reason;
		std::string explanation;
	} http_status_t;

	typedef struct request_data_s {
		std::string clientPath; // ex: google.com/drive/lol -> /drive/lol
		std::string requestedPath; // path on the server of the file to send
		Location location;

		bool indexAppended;

		std::string fileName;
		std::string fileExtension;

		std::string cgiQuery;
	} request_data_t;

	typedef struct tmp_file_s {
		int fd;
		std::string name;
	} tmp_file_t;

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

	// FILES
	bool file_exists(std::string const &path);
	bool file_is_reg(std::string const &path);
	bool file_is_dir(std::string const &path);
	bool make_tmp_file(tmp_file_t &tft);
	void close_tmp_file(ws::tmp_file_t const &tft);
	size_t get_file_size(std::ifstream &stream);
	std::string get_file_contents(std::ifstream &stream, int fileSize);
	std::string html_list_dir(std::string const &path, std::string &relativePath);

	// PARSING
	bool ft_in_charset(char const c, const std::string &charset);
	std::vector<std::string> splitStr(const std::string &str, std::string const &relativePath);

	//MIME TYPES
	std::string mimeTypeFromExtension(std::string extension);
}

#endif
