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

int ws::check_error_page_key(std::string key) {
	int	check;
	int count = 0;
	const char* tmp_c = key.data();
	if (isdigit(tmp_c[0]))
		return 1;
	else
		return 0;
}

void ws::parse_server_line(config_parsing_t &cpt, Server &server) {
    std::cout << "\tserver line: " << cpt.line << std::endl;
    char *cstr = new char [cpt.line.length()+1];
    strcpy (cstr, cpt.line.c_str());
    char *p = ::strtok (cstr, " ");
	std::string str = std::string(p);
	// A mettre dans une autre fonction qui fera également les checks avant parsing
	std::map<std::string, confValues> matchValues;
	matchValues["name"] = NAME;
	matchValues["listen"] = LISTEN;
	matchValues["client_max_body_size"] = CLIENT_MAX_BODY;
	matchValues["autoindex"] = AUTOINDEX;
	matchValues["method"] = METHOD;
	matchValues["cgi"] = CGI;
	matchValues["index"] = INDEX;
	matchValues["error_page"] = ERROR_PAGE;
	matchValues["upload"] = UPLOAD;
	matchValues["root"] = ROOT;
	// 
	int val;
	std::string key = "NULL";
	std::string path = "NULL";
	ServerInfo serverInfo = server.getServerInfo();
	switch (matchValues[str])
	{
		case NAME:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				serverInfo.setServerName(p);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - NAME" << std::endl;
			break;

		case LISTEN:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				ServerSocket socketInfo = server.getSocket();
				socketInfo.setIp(p);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - IP" << std::endl;
			break;
		
		case CLIENT_MAX_BODY:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				int p_tmp = atoi(p);
				serverInfo.setMaxBodySize(p_tmp); // Créer un translateur de Mb ou Gb en Octet
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - CLIENT_MAX_BODY" << std::endl;
			break;

		case AUTOINDEX:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				if (strcmp(p, "on") == 0)
					serverInfo.setAutoIndex(true);
				else
					serverInfo.setAutoIndex(false);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - AUTOINDEX" << std::endl;
			break;

		case METHOD:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				serverInfo.addtoMethod(p);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - METHOD" << std::endl;
			break;
		
		case CGI:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				if (p[0] == '.')
					key = std::string(p);
				else
					path = std::string(p);
				if (key != "NULL" && path != "NULL")
					serverInfo.addToCGIS(key, path);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - CGI" << std::endl;
			break;

		case INDEX:
			p = strtok(NULL, " ,|;");
			while (p!=0)
			{
				serverInfo.setIndexPath(p);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - INDEX" << std::endl;
			break;

		case ERROR_PAGE:
			p = strtok(NULL, " ,|;");
			while (p!=0)
			{
				if (check_error_page_key(std::string(p)))
					val = atoi(p);
				else
					path = std::string(p);
				if (val != 0 && path != "NULL")
					serverInfo.addErrorPage(val, path);
				p = strtok(NULL, " ,|;");
			}
			std::cout << "OK - ERROR_PAGE" << std::endl;
			break;

		case UPLOAD:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				serverInfo.setUploadPath(p);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - UPLOAD" << std::endl;
			break;

		case ROOT:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				serverInfo.setRootPath(p);
				p = strtok(NULL," ,|;");
			}
			std::cout << "OK - ROOT" << std::endl;
			break;
			break;

		default:
			std::cout << "0 - Error Value" << std::endl;
			break;
	}
	delete[] cstr;
}

void ws::parse_location_line(config_parsing_t &cpt, Location &location) {
    std::cout << "\t\tlocation line: " << cpt.line << std::endl;
}

void ws::check_location_path(std::string const &path) {
	std::cout << "\t\tpath: " << path << std::endl;
	std::ifstream test(path);
	if (!test)
    	std::cout << "The file doesn't exist" << std::endl;
	// Simple Check à implémenter plus tard
}

int ws::parse_config(std::string const &name, std::vector<Server*> &servers) {
    config_parsing_t cpt;
    Server *server = NULL;
    Location *location = NULL;
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

                    location = new Location();
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