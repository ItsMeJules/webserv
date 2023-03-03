#include "utils.hpp"

#include <iostream>

int ws::stoi(std::string const &str) {
	int nbr;
	std::stringstream(str) >> nbr;
	return nbr;
}

unsigned int ws::hextoi(std::string const &str) {
	unsigned int x = 0;
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

std::string &ws::skip_chars(std::string &str, std::string const &toSkip) {
    int i = 0;
    while (toSkip.find(str[i]) != std::string::npos)
        i++;
    str.erase(0, i);
    return str;
}

bool ws::string_in_range(std::string const &range, std::string const &str, size_t npos) {
    for (std::string::size_type i = 0; i < str.size() && npos != 0; i++, npos--) {
        if (range.find(str[i]) == std::string::npos)
            return false;
    }
	return true;
}

char *ws::char_array(std::string const &str, int end, int begin) {
	return (new std::vector<char>(str.data() + begin, str.data() + end))->data();
}

int ws::pos_in_vec(std::string const &str, std::vector<char> const &vec) {
	std::vector<char>::size_type i;
	std::vector<char>::size_type j;

	for (i = 0; i < vec.size(); i++) {
		j = 0;
		if (vec[i] == str[j]) {
			while (j < str.size() && i + j < vec.size() && vec[i + j] == str[j])
				j++;
			if (j == str.size())
				return i;
		}
	}
	return -1;
}

int ws::pos_in_vec_from_end(std::string const &str, std::vector<char> const &vec) {
	int i;
	int j;

	for (i = vec.size() - 1; i >= 0; i--) {
		j = str.size() - 1;
		if (vec[i] == str[j]) {
			while (j > 0 && i - j > 0 && vec[i - (str.size() - 1 - j)] == str[j])
				j--;
			if (j == 0)
				return i - (str.size() - 1 - j);
		}
	}
	return -1;
}

void ws::log(int const &level, std::string const &prefix, std::string const &message, const bool &_errno) {
	const int lvl = ws::LOG_LVL & level;
	std::string color;

	if (ws::LOG_LVL & ws::LOG_LVL_NONE || !lvl)
		return ;

	if (lvl == ws::LOG_LVL_ERROR)
		color = ws::C_RED;
	else if (lvl == ws::LOG_LVL_INFO || lvl == ws::LOG_LVL_ALL)
		color = ws::C_YELLOW;
	else if (lvl == ws::LOG_LVL_SUCCESS)
		color = ws::C_LIME;
	else if (lvl == ws::LOG_LVL_DEBUG)
		color = C_SILVER;
	else if (lvl == ws::LOG_LVL_PARSING)
		color = C_SILVER;

	std::cout << "(LOGLVL: " << level << ") " << ws::C_AQUA << prefix << (prefix.empty() ? "" : " ") << color << message << ws::C_RESET << std::endl;
	if (_errno)
		std::cout << "\n error: " << strerror(errno) << std::endl;
}

bool ws::file_exists(std::string const &path) {
	struct stat fileInfo;
	return stat(path.c_str(), &fileInfo) == 0;
}

bool ws::file_is_reg(std::string const &path) {
	struct stat fileInfo;
	stat(path.c_str(), &fileInfo);
	return S_ISREG(fileInfo.st_mode);
}

bool ws::file_is_dir(std::string const &path) {
	struct stat fileInfo;
	stat(path.c_str(), &fileInfo);
	return S_ISDIR(fileInfo.st_mode);
}

bool ws::make_tmp_file(tmp_file_t &tft) {
	tft.name += ".XXXXXX";

    std::vector<char> path(tft.name.begin(), tft.name.end());
    path.push_back('\0');

	tft.fd = mkstemp(&path[0]);
	if (tft.fd != 1) {
        tft.name.assign(path.begin(), path.end() - 1);
		close(tft.fd);
	}
	return tft.fd != -1;
}

void ws::close_tmp_file(ws::tmp_file_t const &tft) {
	close(tft.fd);
	unlink(tft.name.c_str());
}

size_t ws::get_file_size(std::ifstream &stream) {
	size_t size;

	stream.seekg(0, std::ifstream::end);
	size = stream.tellg();
	stream.seekg(0);
	return size;
}

std::string ws::get_file_contents(std::ifstream &stream, int fileSize) {
	std::string fileContent = std::string(fileSize, ' ');
	stream.read(&fileContent[0], fileSize);
	return fileContent;
}

std::string ws::html_list_dir(std::string const &path, std::string &relativePath) {
	DIR *dir;
	struct dirent *ent;
	std::string html = "<!DOCTYPE html><html><head><title>WebServ Autoindex</title></head><body><ul>";

	if (relativePath[relativePath.size() - 1] != '/')
		relativePath += "/";

	if ((dir = opendir(path.c_str())) != NULL) {
		html += "</a></li>";
		html += "<li><a href=\"";
		html += relativePath + "..";
		html += "\">";
		html += "..";
		html += "</a></li>";
		while ((ent = readdir(dir)) != NULL) {
			std::string d_name = std::string(ent->d_name);

			if (d_name == ".." || d_name == ".")
				continue;

			html += "<li><a href=\"";
			html += relativePath + ent->d_name;
			html += "\">";
			html += ent->d_name;
			html += "</a></li>";
		}
		closedir(dir);
	}
	html += "</ul></body></html>";
	return html;
}


bool	ws::ft_in_charset(char const c, const std::string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}

std::vector<std::string> ws::splitStr(const std::string &str, const std::string &charset)
{
	std::vector<std::string> res;
	std::string			tmp;
	size_t			i;

	i = 0;
	while (i < str.length())
	{
		while (i < str.length() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.length())
		{
			tmp = "";
			while (i < str.length() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return res;
}

std::string ws::mimeTypeFromExtension(std::string extension) {
	std::string mime = "";

	for (int i = 0; i < ws::NB_MIME; i++) {
		if (extension == ws::MIME_EXTENSIONS[i])
			mime = ws::MIME_TYPES[i];
	}

	if (mime.size() == 0)
		return ("text/plain");
	return mime;
}
