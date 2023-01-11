#include "config_parser.hpp"

// TODO: lancer des exceptions
ws::ConfigBlockType ws::get_block_type(std::string line) {
    ws::skip_chars(line, ws::WHITE_SPACES); // skips the spaces before the block declaration
    if (line.rfind("server", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 6), ws::WHITE_SPACES); //skips the spaces after the block declaration
        if (line[0] != '{' || line.size() > 1)
            return ConfigBlockType::NONE;
        return ConfigBlockType::SERVER;
    } else if (line.rfind("location", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 8), ws::WHITE_SPACES); //skips the spaces before the path
        if (line[0] == '/') {
            ws::skip_chars(line, ws::PATH_LETTERS); //skips the path
            ws::skip_chars(line, ws::WHITE_SPACES); //skips the spaces after the path
            if (line[0] != '{' || line.size() > 1)
                return ConfigBlockType::NONE;
            return ConfigBlockType::LOCATION;
        }
    }
    return ConfigBlockType::NONE;
}

int ws::parse_server_block(config_parsing_t &cpt, Server &server) {
    cpt.blockLevel++;
    while (std::getline(cpt.file, cpt.line)) {

    }
    cpt.blockLevel--;
}

int ws::parse_location_block(config_parsing_t &cpt, ServerInfo::Location location) {
    cpt.blockLevel++;
    while (std::getline(cpt.file, cpt.line)) {

    }
    cpt.blockLevel--;
}

int ws::parse_config(std::string const &name, std::vector<Server> &servers) {
    config_parsing_t cpt;

    cpt.file.open(name.c_str());
    cpt.blockLevel = 0;

    while (std::getline(cpt.file, cpt.line)) {
        if (get_block_type(cpt.line) == ConfigBlockType::SERVER) {
            Server server;
            parse_server_block(cpt, server);
            if (get_block_type(cpt.line) == ConfigBlockType::LOCATION) {
                ServerInfo::Location location(cpt.line);
                parse_location_block(cpt, location);
            }
        }
    }
}