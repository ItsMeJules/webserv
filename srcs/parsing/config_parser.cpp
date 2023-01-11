#include "config_parser.hpp"

void ws::check_opening_bracket(std::string line, std::string fullLine) {
    if (line[0] != '{' || line.size() > 1)
        throw std::invalid_argument("Line : \"" + fullLine + "\" is invalid! ("
                                    + (line.size() > 1 ? "extra chars after bracket" : "line should end with a bracket") + ")");
}

int ws::check_closing_bracket(std::string line, std::string fullLine) {
    if (line[0] == '}') {
        if (line.size() == 1)
            return 1;
        throw new std::invalid_argument("Line : \"" + cpt.line + "\" is invalid. There are extra characters after the closing bracket!");
    }
    return 0;
}

ws::ConfigBlockType ws::get_block_type(std::string line) {
    std::string clone = line;
    ws::skip_chars(line, ws::WHITE_SPACES); // skips the spaces before the block declaration
    if (line.rfind("server", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 6), ws::WHITE_SPACES); //skips the spaces after the block declaration
        ws::check_opening_bracket(line, clone);
        return SERVER;
    } else if (line.rfind("location", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 8), ws::WHITE_SPACES); //skips the spaces before the path
        if (line[0] == '/') {
            ws::skip_chars(line, ws::PATH_LETTERS); //skips the path
            ws::skip_chars(line, ws::WHITE_SPACES); //skips the spaces after the path
            ws::check_opening_bracket(line, clone);
            return LOCATION;
        } else
            throw std::invalid_argument("Location path must be absolute! String: \"" + clone + "\" is invalid!");
    }
    throw std::invalid_argument("Block line \"" + clone + "\" could not be recognized!");
}

ws::ConfigBlockType ws::parse_server_block(config_parsing_t &cpt, Server &server) {
    cpt.blockLevel++;
    while (std::getline(cpt.file, cpt.line)) {
        if (get_block_type(cpt.line) == LOCATION)
            return LOCATION;
        ws::skip_chars(cpt.line, ws::WHITE_SPACES); // deletes first whitespace of the line.
        if (ws::check_closing_bracket(cpt.line, cpt.line))
            break ;
        parse_server_line(cpt, server);
    }
    cpt.blockLevel--;
    return END;
}

void ws::parse_server_line(config_parsing_t &cpt, Server &server) {

}

ws::ConfigBlockType ws::parse_location_block(config_parsing_t &cpt, ServerInfo::Location location) {
    cpt.blockLevel++;
    while (std::getline(cpt.file, cpt.line)) {
        ws::skip_chars(cpt.line, ws::WHITE_SPACES); // deletes first whitespace of the line.
        if (ws::check_closing_bracket(cpt.line, cpt.line))
            break ;
        parse_location_line(cpt, location);
    }
    cpt.blockLevel--;
    return END;
}

void ws::parse_location_line(config_parsing_t &cpt, ServerInfo::Location &location) {

}

int ws::parse_config(std::string const &name, std::vector<Server> &servers) {
    config_parsing_t cpt;

    cpt.file.open(name.c_str());
    cpt.blockLevel = 0;

    while (std::getline(cpt.file, cpt.line)) {
        if (get_block_type(cpt.line) == SERVER) {
            Server server;
            if (parse_server_block(cpt, server) == LOCATION) {
                ServerInfo::Location location(cpt.line);
                parse_location_block(cpt, location);
            }
        }
    }
    return 1;
}