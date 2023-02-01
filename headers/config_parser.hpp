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
    };

    typedef struct config_parsing_s {
        std::ifstream file;
        std::string line;
        int lineNumber;
        int blockLevel;
        std::map<std::string, confValues> configKeys;
    } config_parsing_t;

    enum ConfigLineType {
        HTTP,
        SERVER,
        LOCATION,
        INFO,
    };

    template<typename To_String>
        std::string to_string(const To_String & value) {    
            std::ostringstream oss;
            oss << value;
            return oss.str();
    }

    bool                        isPathExist(const std::string &s);

    ConfigLineType              getBlockType(config_parsing_t &cpt, std::string line);

    int		                    checkAutoIndex(std::string index, ServerInfo &serverInfo);
    int		                    checkClientMaxBodySize(std::string size, ServerInfo &serverInfo);
	int		                    checkClosingBracket(config_parsing_t const &cpt);
    int		                    checkFileExtension(std::string file);
    int		                    checkIpKey(std::string key);
	int 	                    checkLocationAutoIndex(std::string index, Location &locationInfo);
    int                         checkPortKey(std::string key);
    int		                    parseConfig(std::string const &name, std::vector<Server*> &servers);
    int		                    parseLocationLine(config_parsing_t &cpt, Location &location);
    int		                    parseServerLine(config_parsing_t &cpt, Server &server);

    void						checkerArguments(size_t nbArguments, size_t argtExpect, std::string argument);
    void                        checkConfiguration(Server *servers);
    void	                    checkOpeningBracket(config_parsing_t const &cpt, std::string const &line);
    void	                    parserInit(std::map<std::string, confValues> &Values);

};

#endif