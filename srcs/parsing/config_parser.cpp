#include "config_parser.hpp"

void ws::check_opening_bracket(config_parsing_t const &cpt, std::string const &line) {
    if (line[0] != '{' || line.size() > 1)
        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ": \"" + line + "\" is invalid! ("
                                    + (line.size() > 1 ? "extra chars on bracket" : "line should end with a bracket") + ").");
}

int ws::check_closing_bracket(config_parsing_t const &cpt) {
    if (cpt.line[0] == '}') {
        if (cpt.line.size() == 1)
            return 1;
        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ": \"" + cpt.line + "\" is invalid. There are extra characters after the closing bracket!");
    }
    return 0;
}

ws::ConfigLineType ws::get_block_type(config_parsing_t &cpt, std::string line) {
    if (line.rfind("server", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 6), ws::WHITE_SPACES); //skips the spaces after the block declaration
        ws::check_opening_bracket(cpt, line);
        return SERVER;
    } else if (line.rfind("location", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 8), ws::WHITE_SPACES); //skips the spaces before the path
        if (line[0] == '/') {
            ws::skip_chars(line, ws::CONFIG_PATH_LETTERS); //skips the path
            ws::skip_chars(line, ws::WHITE_SPACES); //skips the spaces after the path
            ws::check_opening_bracket(cpt, line);
            return LOCATION;
        } else
            throw std::invalid_argument("Location path must be absolute! String: \"" + cpt.line + "\" is invalid!");
    } else {
        size_t declarationEndPos = line.find_first_of(ws::WHITE_SPACES);
        if (ws::string_in_range(ws::CONFIG_INFO_LETTERS, line, declarationEndPos))
            return INFO;
    }
    throw std::invalid_argument("Block line \"" + line + "\" could not be recognized!");
}

void ws::parse_server_line(config_parsing_t &cpt, Server &server) {
    std::cout << "\tserver line: " << cpt.line << std::endl;
    char *cstr = new char [cpt.line.length()+1];
    std::strcpy (cstr, cpt.line.c_str());
    char *p = std::strtok (cstr, " ");
	std::string str = std::string(p);
	std::map<std::string, confValues> matchValues;
	matchValues["listen"] = LISTEN;
	matchValues["client_max_body_size"] = CLIENT_MAX_BODY;
	matchValues["autoindex"] = AUTOINDEX;
	matchValues["method"] = METHOD;
	matchValues["cgi"] = CGI;
	matchValues["index"] = INDEX;
	switch (matchValues[str])
	{
		case LISTEN:
			p = std::strtok(NULL," ,|;");
			while (p!=0)
			{
				std::cout << "\t\tvalue: "<< p << '\n';
				p = std::strtok(NULL," ,|;");
			}
			break;
		
		case CLIENT_MAX_BODY:
			p = std::strtok(NULL," ,|;");
			while (p!=0)
			{
				std::cout << "\t\tvalue: "<< p << '\n';
				p = std::strtok(NULL," ,|;");
			}
			break;

		case AUTOINDEX:
			p = std::strtok(NULL," ,|;");
			while (p!=0)
			{
				std::cout << "\t\tvalue: "<< p << '\n';
				p = std::strtok(NULL," ,|;");
			}
			break;

		case METHOD:
			p = std::strtok(NULL," ,|;");
			while (p!=0)
			{
				std::cout << "\t\tvalue: "<< p << '\n';
				p = std::strtok(NULL," ,|;");
			}
			break;
		
		case CGI:
			p = std::strtok(NULL," ,|;");
			while (p!=0)
			{
				std::cout << "\t\tvalue: "<< p << '\n';
				p = std::strtok(NULL," ,|;");
			}
			break;

		case INDEX:
			p = std::strtok(NULL," ,|;");
			while (p!=0)
			{
				std::cout << "\t\tvalue: "<< p << '\n';
				p = std::strtok(NULL," ,|;");
			}
			break;
		default:
			std::cout << "0 - Error Value" << std::endl;
			break;
	}
    // while (p!=0)
    // {
    // 	std::cout << "\t\tkeyword: "<< p << '\n';
	// 	p = std::strtok(NULL," ,|;");
	// }
	
	delete[] cstr;
}

void ws::parse_location_line(config_parsing_t &cpt, ServerInfo::Location &location) {
    std::cout << "\t\tlocation line: " << cpt.line << std::endl;
}

void ws::check_location_path(std::string const &path) {

}

int ws::parse_config(std::string const &name, std::vector<Server*> &servers) {
    config_parsing_t cpt;
    Server *server = NULL;
    ServerInfo::Location *location = NULL;
    ConfigLineType lineType;

    cpt.file.open(name.c_str());
    cpt.lineNumber = 0;
    cpt.blockLevel = 0;

    while (std::getline(cpt.file, cpt.line)) {
        cpt.lineNumber++;
        ws::skip_chars(cpt.line, ws::WHITE_SPACES); // deletes first whitespaces of the line.
        if (cpt.line.empty())
            continue ;
        if (ws::check_closing_bracket(cpt)) {
            if (cpt.blockLevel == 1)
                server = NULL;
            else if (cpt.blockLevel == 2)
                location = NULL;
            std::cout << (cpt.blockLevel == 2 ? "\t" : "");
            std::cout << "closing bracket" << std::endl;
            cpt.blockLevel--;
            continue ;
        }

        lineType = get_block_type(cpt, cpt.line);
        switch (cpt.blockLevel) {
            case 0: //we are not inside any block
                if (lineType != SERVER)
                    throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", only server blocks can be found at this level.");
                std::cout << "new server line: " << cpt.line << std::endl;
                server = new Server();
                servers.push_back(server);
                cpt.blockLevel++;
                break ;
            case 1: //we are inside server block
                if (lineType == LOCATION && location == NULL) {
                    std::cout << "\tnew location line: " << cpt.line << std::endl;

                    ws::skip_chars(cpt.line.erase(0, 8), ws::WHITE_SPACES); //we erase "location", then we skip the spaces after location
                    cpt.line.erase(cpt.line.find_first_of(ws::WHITE_SPACES)); //we erase any trailing characters after the path
                    ws::check_location_path(cpt.line);

                    if (server->getServerInfo().getLocations().count(cpt.line) != 0)
                        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", location block \"" + cpt.line + "\" already declared.");

                    location = new ServerInfo::Location();
                    server->getServerInfo().addLocation(cpt.line, location);
                    cpt.blockLevel++;
                } else if (lineType == INFO)
                    parse_server_line(cpt, *server);
                break ;
            case 2: //we are inside location block
                if (lineType != INFO)
                    throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ": \"" + cpt.line +
                                                "\" is invalid. There can't be a location block inside another location block.");
                    parse_location_line(cpt, *location);
                break ;
            default:
                break ;
        }
    }
    if (cpt.blockLevel != 0)
        throw std::invalid_argument("Missing closing bracket!");
    std::cout << "whole file is parsed" << std::endl;
    return 0;
}