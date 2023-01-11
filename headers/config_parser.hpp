#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <iostream>
# include <vector>
# include <fstream>

# include "Server.hpp"
# include "ServerInfo.hpp"

namespace ws {
    typedef struct config_parsing_s {
        std::ifstream file;
        std::string line;
        int blockLevel;
    } config_parsing_t;

    enum ConfigBlockType {
        SERVER,
        LOCATION,
        END,
        NONE
    };

    int parse_config(std::string const &name, std::vector<Server> &servers);
    ConfigBlockType get_block_type(std::string line);
    ws::ConfigBlockType parse_server_block(config_parsing_t &cpt, Server &server);
    ws::ConfigBlockType parse_location_block(config_parsing_t &cpt, ServerInfo::Location location);
    void parse_server_line(config_parsing_t &cpt, Server &server);
    void parse_location_line(config_parsing_t &cpt, ServerInfo::Location &location);

    void check_opening_bracket(std::string line, std::string fullLine);
    int check_closing_bracket(std::string line, std::string fullLine);


};

#endif