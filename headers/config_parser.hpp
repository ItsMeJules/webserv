#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <fstream>
# include <locale>
# include <string>
# include <string.h>
# include <iostream>

# include "Server.hpp"
# include "ServerInfo.hpp"
# include "Constants.hpp"

namespace ws {

    enum confValues {
        ERROR,
        NAME,
        LISTEN,
        CLIENT_MAX_BODY,
        AUTOINDEX,
        METHOD,
        CGI,
        INDEX,
        ERROR_PAGE,
        UPLOAD,
        ROOT,
        REWRITE
    };

    typedef struct config_parsing_s {
        std::ifstream file;
        std::string line;
        int lineNumber;
        int blockLevel;
        std::map<std::string, confValues> configKeys;
    } config_parsing_t;

    enum ConfigLineType {
        HTML,
        SERVER,
        LOCATION,
        INFO,
    };

    ConfigLineType              get_block_type(config_parsing_t &cpt, std::string line);

    int		                    parse_config(std::string const &name, std::vector<Server*> &servers);
    int		                    checkIpKey(std::string key);
    int                         checkPortKey(std::string key);
    int                         checkErrorKey(std::string key);
    int		                    parse_server_line(config_parsing_t &cpt, Server &server);
    int		                    parseLocationLine(config_parsing_t &cpt, Location &location);
    int		                    checkFileExtension(std::string file);
    int		                    checkClientMaxBodySize(std::string size, ServerInfo &serverInfo);
    int		                    checkAutoIndex(std::string index, ServerInfo &serverInfo);
	int 	                    checkAutoIndex(std::string index, Location &locationInfo);
    int		                    checkMethod(std::string method, ServerInfo &serverInfo);
	int		                    checkMethod(std::string method, Location &locationInfo);
    void	                    parserInit(std::map<std::string, confValues> &Values);
    std::vector<std::string>    splitStr(const std::string &str, const std::string &charset);
    bool	                    ft_in_charset(char const c, const std::string &charset);

	void	                    checkPath(std::string const &path);
    void	                    check_opening_bracket(config_parsing_t const &cpt, std::string const &line);
    int		                    check_closing_bracket(config_parsing_t const &cpt);

};

#endif