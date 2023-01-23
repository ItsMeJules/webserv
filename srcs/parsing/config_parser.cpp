#include "config_parser.hpp"
#include <fstream>
#include <iostream>

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
	if (line.rfind("html", 0) != std::string::npos) {
		ws::skip_chars(line.erase(0, 4), ws::WHITE_SPACES); //skips the spaces after the block declaration
        ws::check_opening_bracket(cpt, line);
        return HTML;
	} else if (line.rfind("server", 0) != std::string::npos) {
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

int ws::checkFileExtension(std::string file) {
	std::ifstream path;
	const char * str = file.c_str();

	path.open(str);
	if (!path) {
		std::cerr << "Problem Configuration Files - Error: Can't open the file" << std::endl;
		return (1); }
	path.close();
	if (file.rfind(".conf") == -1 || !(file.size() > 5)) {
		std::cerr << "Problem Configuration Files - The File extension is not .conf" << std::endl;
		return (1);
	}
	return (0);
}

int ws::checkPort(int port, ServerSocket &socketInfo) {
	if (port > 0 && port < 65535) {
		socketInfo.setPort(port);
		return (0);
	}
	else {
		std::cerr << "Problem Configuration Files - LISTEN" << std::endl;
		return (1);
	}
}

int ws::checkClientMaxBodySize(std::string size, ServerInfo &serverInfo) {
	int len = size.size();
	int i = 0;
	int mult = 0;
	int sizeBody = atoi(size.c_str());

	for (; i < len; i++)
		if (!std::isdigit(size[i]))
			break;

	if (i != len)
	{
		if (size[i] == 'G' && i == len - 1)
			sizeBody *= 1000000000;
		else if (size[i] == 'M' && i == len - 1)
			sizeBody *= 1000000;
		else if (size[i] == 'K' && i == len - 1)
			sizeBody *= 1000;
		else {
			std::cerr << "Problem Configuration Files - CLIENTMAXBODYSIZE" << std::endl;
			return (1);
		}
	}
	serverInfo.setMaxBodySize(sizeBody);
	return (0);
}

int ws::checkAutoIndex(std::string index, ServerInfo &serverInfo) {
	if (index.compare("on") == 0) {
		serverInfo.setAutoIndex(true);
		return (0);
	}
	else if (index.compare("off") == 0) {
		serverInfo.setAutoIndex(false);
		return (0);
	}
	else {
		std::cerr << "Problem Configuration Files - AUTOINDEX" << std::endl;
		return (1);
	}
}

int ws::checkMethod(std::string method, ServerInfo &serverInfo) {
	if ((method.compare("GET") == 0) || (method.compare("POST") == 0) || (method.compare("DELETE") == 0)) {
		serverInfo.addtoMethod(method);
		return (0);
	}
	else {
		std::cerr << "Problem Configuration Files - METHOD" << std::endl;
		return (1);
	}
}

int ws::check_error_page_key(std::string key) {
	int	check;
	int key_err = 0;
	const char* tmp_c = key.data();
	if (isdigit(tmp_c[0]))
		return (1);
	else
		return 0;
}

int ws::parse_server_line(config_parsing_t &cpt, Server &server) {
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
	ServerSocket socketInfo = server.getSocket();
	switch (matchValues[str])
	{
		case NAME:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				serverInfo.setServerName(p);
				p = strtok(NULL," ,|;");
			}
			break;

		case LISTEN: // Checker --> OK
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				int p_tmp = atoi(p);
				if (checkPort(p_tmp, socketInfo))
					return (1);
				p = strtok(NULL," ,|;");
			}
			break;
		
		case CLIENT_MAX_BODY: // Checker --> OK
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				std::string size = std::string(p);
				if (checkClientMaxBodySize(size, serverInfo))
					return (1);
				p = strtok(NULL," ,|;");
			}
			break;

		case AUTOINDEX: // Checker --> OK
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				std::string index = std::string(p);
				if (checkAutoIndex(index, serverInfo))
					return (1);
				p = strtok(NULL," ,|;");
			}
			break;

		case METHOD: // Checker --> OK
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				std::string method = std::string(p);
				if (checkMethod(method, serverInfo))
					return (1);
				p = strtok(NULL," ,|;");
			}
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
			break;

		case INDEX:
			p = strtok(NULL, " ,|;");
			while (p!=0)
			{
				serverInfo.setIndexPath(p);
				p = strtok(NULL," ,|;");
			}
			break;
		
		case ERROR_PAGE:
			p = strtok(NULL, " ,|;");
			while (p!=0)
			{
				std::string value = std::string(p);
				int size = value.size();
				if ((size == 3) && (check_error_page_key(value) == 1))
					val = atoi(p);
				if (val <= 600 && val >= 100)
					path = std::string(p);
				else {
					std::cerr << "Problem Configuration Files - ERROR_PAGE" << std::endl;
					return 1;
				}
				p = strtok(NULL, " ,|;");
			}
			break;

		case UPLOAD:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				serverInfo.setUploadPath(p);
				p = strtok(NULL," ,|;");
			}
			break;

		case ROOT:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				serverInfo.setRootPath(p);
				p = strtok(NULL," ,|;");
			}
			break;

		default:
			std::cerr << "Problem Configuration Files - DEFAULT_ERROR" << std::endl;
			break;
	}
	delete[] cstr;
	return 0;
}

void ws::parse_location_line(config_parsing_t &cpt, Location &location) {
    std::cout << "\t\tlocation line: " << cpt.line << std::endl;
	std::cout << "\tserver line: " << cpt.line << std::endl;
    char *cstr = new char [cpt.line.length()+1];
    strcpy (cstr, cpt.line.c_str());
    char *p = ::strtok (cstr, " ");
	std::string str = std::string(p);
	// A mettre dans une autre fonction qui fera également les checks avant parsing
	std::map<std::string, confValues> matchValues;
	matchValues["autoindex"] = AUTOINDEX;
	matchValues["method"] = METHOD;
	matchValues["index"] = INDEX;
	matchValues["upload"] = UPLOAD;
	matchValues["root"] = ROOT;
	matchValues["rewrite"] = REWRITE;

	Location loc = location;
	switch (matchValues[str])
	{
		case INDEX:
			p = strtok(NULL, " ,|;");
			while (p!=0)
			{
				loc.setIndexPath(p);
				p = strtok(NULL," ,|;");
			}
			break;

		case AUTOINDEX:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				if (strcmp(p, "on") == 0)
					loc.setAutoIndex(true);
				else
					loc.setAutoIndex(false);
				p = strtok(NULL," ,|;");
			}
			break;

		case METHOD:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				loc.addtoMethod(p);
				p = strtok(NULL," ,|;");
			}
			break;

		case UPLOAD:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				loc.setUploadPath(p);
				p = strtok(NULL," ,|;");
			}
			break;

		case REWRITE:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				loc.setRewritePath(p);
				p = strtok(NULL," ,|;");
			}
			break;

		case ROOT:
			p = strtok(NULL," ,|;");
			while (p!=0)
			{
				loc.setRootPath(p);
				p = strtok(NULL," ,|;");
			}
			break;

		default:
			std::cout << "0 - Error Value" << std::endl;
			break;
	}
	delete[] cstr;



}

void ws::check_location_path(std::string const &path) {
	// std::cout << "\t\tpath: " << path << std::endl;
	// std::ifstream test(path);
	// if (!test)
    // 	std::cout << "The file doesn't exist" << std::endl;
	// Simple Check à implémenter plus tard
}

int ws::parse_config(std::string const &name, std::vector<Server*> &servers) {
    config_parsing_t cpt;
    Server *server = NULL;
    Location *location = NULL;
    ConfigLineType lineType;

	if (ws::checkFileExtension(name))
		return 1;
	cpt.file.open(name.c_str());
    cpt.lineNumber = 0;
    cpt.blockLevel = 0;

    while (std::getline(cpt.file, cpt.line)) {
        cpt.lineNumber++;
        ws::skip_chars(cpt.line, ws::WHITE_SPACES); // deletes first whitespaces of the line.
        if (cpt.line.empty())
            continue ;
        if (ws::check_closing_bracket(cpt)) {
            if (cpt.blockLevel == 2)
                server = NULL;
            else if (cpt.blockLevel == 3)
                location = NULL;
            std::cout << (cpt.blockLevel == 3 ? "\t" : "");
            std::cout << "closing bracket" << std::endl;
            cpt.blockLevel--;
            continue ;
        }

        lineType = get_block_type(cpt, cpt.line);
        switch (cpt.blockLevel) {
			case 0: // check if we are inside the HTML, else it's dead
				if (lineType != HTML)
                    throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", only html blocks can be found at this level.");
				std::cout << "html line: " << cpt.line <<std::endl;
				cpt.blockLevel++;
				break;
            case 1: //we are inside the html block
                if (lineType != SERVER)
                    throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", only server blocks can be found at this level.");
                std::cout << "\tnew server line: " << cpt.line << std::endl;
                server = new Server();
                servers.push_back(server);
                cpt.blockLevel++;
                break ;
            case 2: //we are inside server block
                if (lineType == LOCATION && location == NULL) {
                    std::cout << "\t\tnew location line: " << cpt.line << std::endl;

                    ws::skip_chars(cpt.line.erase(0, 8), ws::WHITE_SPACES); //we erase "location", then we skip the spaces after location
                    cpt.line.erase(cpt.line.find_first_of(ws::WHITE_SPACES)); //we erase any trailing characters after the path
                    ws::check_location_path(cpt.line);

                    if (server->getServerInfo().getLocations().count(cpt.line) != 0)
                        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", location block \"" + cpt.line + "\" already declared.");

                    location = new Location();
                    server->getServerInfo().addLocation(cpt.line, location);
                    cpt.blockLevel++;
                } else if (lineType == INFO) {
					int i = parse_server_line(cpt, *server);
					if (i == 1)
						return 1;
				}     
                break ;
            case 3: //we are inside location block
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