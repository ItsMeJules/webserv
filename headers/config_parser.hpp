#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <iostream>
# include <vector>
# include <fstream>
# include <locale>
# include <string>
# include <string.h>
# include <iostream>

# include "Server.hpp"
# include "ServerInfo.hpp"
# include "Constants.hpp"

namespace ws {

    typedef struct config_parsing_s {
        std::ifstream file;
        std::string line;
        int lineNumber;
        int blockLevel;
    } config_parsing_t;

    enum ConfigLineType {
        SERVER,
        LOCATION,
        INFO,
    };

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

    ConfigLineType get_block_type(config_parsing_t &cpt, std::string line);

    int parse_config(std::string const &name, std::vector<Server*> &servers);
    int  check_error_page_key(std::string key);
    int parse_server_line(config_parsing_t &cpt, Server &server);
    void parse_location_line(config_parsing_t &cpt, Location &location);
    int checkFileExtension(std::string file);
    int checkPort(int port, ServerSocket &socketInfo);
    int checkClientMaxBodySize(std::string size, ServerInfo &serverInfo);
    int checkAutoIndex(std::string index, ServerInfo &serverInfo);
    int checkMethod(std::string method, ServerInfo &serverInfo);


    void check_location_path(std::string const &path);
    void check_opening_bracket(config_parsing_t const &cpt, std::string const &line);
    int check_closing_bracket(config_parsing_t const &cpt);

};

#endif