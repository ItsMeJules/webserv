#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <iostream>
# include <vector>
# include <fstream>

# include "Server.hpp"
# include "ServerInfo.hpp"

namespace ws {
    int parse_config(std::string name, std::vector<Server> &servers);
};

#endif