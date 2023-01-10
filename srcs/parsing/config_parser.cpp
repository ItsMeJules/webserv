#include "config_parser.hpp"

std::string ws::get_block_type(std::string line) {
    line = ws::ltrim(line);


    return line;
}

int ws::parse_config(std::string const &name, std::vector<Server> &servers) {
    std::ifstream file(name.c_str());
    std::string line;
    std::string serverLine;
    std::string locationLine;

    while (std::getline(file, line)) {
        if (ws::ltrim(line) == "server {") {
            Server server;
            while (std::getline(file, serverLine)) {
                if (get_block_type(serverLine) == "location") {
                    ServerInfo::Location location();
                    while (std::getline(file, locationLine)) {

                    }
                }
            }
        }
    }
}