#include "config_parser.hpp"

void ws::checkOpeningBracket(config_parsing_t const &cpt, std::string const &line) {
    if (line[0] != '{' || line.size() > 1)
        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ": \"" + line + "\" is invalid! ("
                                    + (line.size() > 1 ? "extra chars on bracket" : "line should end with a bracket") + ").");
}

int ws::checkClosingBracket(config_parsing_t const &cpt) {
    if (cpt.line[0] == '}') {
        if (cpt.line.size() == 1)
            return 1;
        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ": \"" + cpt.line + "\" is invalid. There are extra characters after the closing bracket!");
    }
    return 0;
}

ws::ConfigLineType ws::getBlockType(config_parsing_t &cpt, std::string line) {
	if (line.rfind("http", 0) != std::string::npos) {
		ws::skip_chars(line.erase(0, 4), ws::WHITE_SPACES); //skips the spaces after the block declaration
        ws::checkOpeningBracket(cpt, line);
        return HTTP;
	} else if (line.rfind("server", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 6), ws::WHITE_SPACES); //skips the spaces after the block declaration
        ws::checkOpeningBracket(cpt, line);
        return SERVER;
    } else if (line.rfind("location", 0) != std::string::npos) {
        ws::skip_chars(line.erase(0, 8), ws::WHITE_SPACES); //skips the spaces before the path
        if (line[0] == '/' || line[0] == '.') {
            ws::skip_chars(line, ws::CONFIG_PATH_LETTERS); //skips the path
            ws::skip_chars(line, ws::WHITE_SPACES); //skips the spaces after the path
            ws::checkOpeningBracket(cpt, line);
            return LOCATION;
		}
    	else
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
	if (file.rfind(".conf") == static_cast<unsigned long>(-1) || !(file.size() > 5)) {
		std::cerr << "Problem Configuration Files - The File extension is not .conf" << std::endl;
		return (1);
	}
	return (0);
}

int ws::checkClientMaxBodySize(std::string size, ServerInfo &serverInfo) {
	int len = size.size();
	int i = 0;
	int sizeBody = atoi(size.c_str());

	for (; i < len; i++)
		if (!std::isdigit(size[i]))
			break;

	if (i != len)
	{
		if (size[i] == 'G' && i == len - 1)
			sizeBody *= GIGA;
		else if (size[i] == 'M' && i == len - 1)
			sizeBody *= MEGA;
		else if (size[i] == 'K' && i == len - 1)
			sizeBody *= KILO;
		else {
			throw std::invalid_argument(size + " must finish with a 'G', 'M' or a 'K' !");
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
		throw std::invalid_argument(index + "need to be params by 'on' or 'off' !");
		return (1);
	}
}

int ws::checkLocationAutoIndex(std::string index, Location &locationInfo) {
	if (index.compare("on") == 0) {
		locationInfo.setAutoIndex(1);
		return (0);
	}
	else if (index.compare("off") == 0) {
		locationInfo.setAutoIndex(0);
		return (0);
	}
	else
		throw std::invalid_argument(index + "need to ne params by 'on' or 'off' !");

	locationInfo.setAutoIndex(index == "on");
	return 0;
}

int ws::checkIpKey(std::string key) {
	const char *tmp_c = key.data();
	int i = 0;
	while (tmp_c[i]) {
		if ((tmp_c[i] >= '0' && tmp_c[i] <= '9' ) || tmp_c[i] == '.')
			i++;
		else
			throw std::invalid_argument("Error, Invalid Listen : Can have only an IP & Port.");
	}
	return 0;
}

int ws::checkPortKey(std::string key) {
	const char *tmp = key.c_str();
	int p_tmp = atoi(tmp);
	int i = 0;

	while (tmp[i])
	{
		if (tmp[i] >= '0' && tmp[i] <= '9')
			i++;
		else
			throw std::invalid_argument("Error, Invalid Listen : Port can only contain number.");
	}
	if (p_tmp < 0 || p_tmp > 65535)
		throw std::invalid_argument("Error, Invalid Listen : Port can only be between 0 and 65535.");
	return 0;
}

void ws::parserInit(std::map<std::string, confValues> &matchValues) {
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
}

void ws::checkerArguments(size_t nbArguments, size_t argtExpect, std::string argument) {
	size_t	sizeArgument = argument.size() - 1;
	std::string expect = to_string(argtExpect);

	if (nbArguments != argtExpect)
		throw std::invalid_argument(to_string(argtExpect) + " Arguments expected.");

	if (argument[sizeArgument] != ';')
		throw std::invalid_argument(argument + " is not a valid name for a server. Missing ';' at the end of the line!");
}

int ws::parseServerLine(config_parsing_t &cpt, Server &server) {
	std::vector<std::string> lineArguments = splitStr(cpt.line, ws::WHITE_SPACES);
	std::vector<std::string> values;
	std::map<std::string, std::string> cgi;
	std::vector<std::string> method;
	std::map<int, std::string> errorPage;
	int	tmp = 0;

	ServerInfo &serverInfo = server.getServerInfo();
	size_t sizeArgumentOne = lineArguments[1].size() - 1;

	for (std::vector<std::string>::iterator it = lineArguments.begin(); it != lineArguments.end(); it++) {
		std::cout << "Argument: " << *it << std::endl;
	}
	if (cpt.configKeys.count(lineArguments[0]) == 0)
		throw std::invalid_argument(lineArguments[0] + " is not a valid key for the config.");

	switch (cpt.configKeys[lineArguments[0]]) {
		case NAME:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			if (ws::string_in_range(lineArguments[1], ws::AL_NUM, sizeArgumentOne))
				throw std::invalid_argument(lineArguments[1] + " is not a valid name for a server. Only alnum chars are accepted!");

			serverInfo.setServerName(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in ServerName: " << serverInfo.getServerName() << std::endl;
			break;

		case LISTEN:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			if (lineArguments[1].find(":") != std::string::npos) {
				values = splitStr(lineArguments[1].substr(0, sizeArgumentOne), ":");
				if (checkIpKey(values[0]) == 0)
					serverInfo.setIp(values[0]);
				if (checkPortKey(values[1]) == 0)
					serverInfo.setPort(ws::stoi(values[1]));
				std::cout << "\tSet in ServerIp: " << serverInfo.getIp() << std::endl;
				std::cout << "\tSet in ServerPort: " << serverInfo.getPort() << std::endl;
			} else {
				if (checkPortKey(lineArguments[1].substr(0, sizeArgumentOne)) == 0)
					serverInfo.setPort(ws::stoi(lineArguments[1].substr(0, sizeArgumentOne)));
				std::cout << "\tSet in ServerPort: " << serverInfo.getPort() << std::endl;
			}
			break;

		case CLIENT_MAX_BODY:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			if (checkClientMaxBodySize(lineArguments[1].substr(0, sizeArgumentOne), serverInfo) != 0)
				throw std::invalid_argument(lineArguments[1].substr(0, sizeArgumentOne) + "must finish with a 'G', 'M' or a 'K' !");

			std::cout << "\tSet in ServerClientBodySizeMax: " << serverInfo.getMaxBodySize() << std::endl;
			break;

		case AUTOINDEX:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			if (checkAutoIndex(lineArguments[1].substr(0, sizeArgumentOne), serverInfo) != 0)
				throw std::invalid_argument(lineArguments[1].substr(0, sizeArgumentOne) + "need to be params by 'on' or 'off' !");
			std::cout << "\tSet in ServerAutoIndex: " << serverInfo.hasAutoindex() << std::endl;
			break;

		case INDEX:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			serverInfo.setIndexPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in ServerAutoIndex: " << serverInfo.getIndexPath() << std::endl;
			break;

		case UPLOAD:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			serverInfo.setUploadPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in ServerUploadPath: " << serverInfo.getUploadPath() << std::endl;
			break;

		case ROOT:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			serverInfo.setRootPath(lineArguments[1].substr(0, sizeArgumentOne));
			if (serverInfo.getRootPath()[0] != '/')
				throw std::invalid_argument("Error on line: " + cpt.line + ", root path must be absolute!");
			std::cout << "\tSet in ServerRootPath: " << serverInfo.getRootPath() << std::endl;
			break;

		case CGI:
			checkerArguments(lineArguments.size(), 3, lineArguments[2]);

			serverInfo.addToCGIS(lineArguments[1], lineArguments[2].substr(0, lineArguments[2].size() - 1));
			cgi = serverInfo.getCgis();
			for(std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it)
			{
				std::cout << "\tSet in ServerCGI : First: " << it->first << " Second: " << it->second << "\n";
			}
			break;

		case METHOD:
			method = serverInfo.getMethod();
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			values = splitStr(lineArguments[1].substr(0, sizeArgumentOne), "|");
			for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (HttpRequest::methods.count(*it) == 0)
					throw std::invalid_argument(*it + " need to be params by 'GET', 'POST' or 'DELETE' !");
				if (std::find(method.begin(), method.end(), *it) != method.end())
					continue;
				else {
					serverInfo.addtoMethod(*it);
				}
			}
			for(std::vector<std::string>::const_iterator it = method.begin(); it != method.end(); ++it)
			{
				std::cout << "\tSet in ServerMethod: " << *it << "\n";
			}
			break;

		case ERROR_PAGE:
			checkerArguments(lineArguments.size(), 3, lineArguments[2]);
			tmp = ws::stoi(lineArguments[1]);
			if (HttpResponse::codes.count(tmp) == 0)
				throw std::invalid_argument("Error, Page_Error : The Error Key doen't exist.");

			serverInfo.addErrorPage(tmp, lineArguments[2].substr(0, lineArguments[2].size() - 1));
			errorPage = serverInfo.getError();
			for(std::map<int, std::string>::const_iterator it = errorPage.begin(); it != errorPage.end(); ++it)
			{
				std::cout << "\tSet in ServerErrorPage : First: " << it->first << " Second: " << it->second << "\n";
			}
			break;

		default:
			throw std::invalid_argument("Default Error, the word " + lineArguments[0] + " doesn't exist.");
			break;
	}
	return 0;
}

int ws::parseLocationLine(config_parsing_t &cpt, Location &location) {
	std::vector<std::string> lineArguments = splitStr(cpt.line, ws::WHITE_SPACES);
	std::vector<std::string> values;
	std::vector<std::string> method;

	size_t sizeArgumentOne = lineArguments[1].size() - 1;

	for (std::vector<std::string>::iterator it = lineArguments.begin(); it != lineArguments.end(); it++) {
		std::cout << "Argument: " << *it << std::endl;
	}

	if (cpt.configKeys.count(lineArguments[0]) == 0)
		throw std::invalid_argument(lineArguments[0] + " is not a valid key for the config.");

	switch (cpt.configKeys[lineArguments[0]]) {
		case INDEX:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			location.setIndexPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in LocationIndex: " << location.getIndexPath() << std::endl;
			break;

		case ROOT:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			location.setRootPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in LocationRoot: " << location.getRootPath() << std::endl;
			break;

		case AUTOINDEX:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);

			if (checkLocationAutoIndex(lineArguments[1].substr(0, sizeArgumentOne), location) != 0)
				throw std::invalid_argument(lineArguments[1].substr(0, sizeArgumentOne) + "need to be params by 'on' or 'off' !");
			std::cout << "\tSet in LocationAutoIndex: " << location.getAutoindex() << std::endl;
			break;

		case METHOD:
			method = location.getMethod();
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			values = splitStr(lineArguments[1].substr(0, sizeArgumentOne), "|");
			for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (HttpRequest::methods.count(*it) == 0)
					throw std::invalid_argument(*it + " need to be params by 'GET', 'POST' or 'DELETE' !");
				if (std::find(method.begin(), method.end(), *it) != method.end())
					continue;
				else
					location.addtoMethod(*it);
			}
			method = location.getMethod();
			for(std::vector<std::string>::const_iterator it = method.begin(); it != method.end(); ++it)
			{
				std::cout << "\tSet in LocationMethod: " << *it << "\n";
			}
			break;

		case UPLOAD:
			checkerArguments(lineArguments.size(), 2, lineArguments[1]);
			location.setUploadPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in LocationUpload: " << location.getUploadPath() << std::endl;
			break;

		default:
			throw std::invalid_argument("Default Error, the word " + lineArguments[0] + " doesn't exist.");
			break;
	}
	return 0;
}

int ws::parseConfig(std::string const &name, std::vector<Server*> &servers) {
    config_parsing_t cpt;
    Server *server = NULL;
    Location *location = NULL;
    ConfigLineType lineType;

	if (ws::checkFileExtension(name))
		return 1;

	parserInit(cpt.configKeys);
	cpt.file.open(name.c_str());
    cpt.lineNumber = 0;
    cpt.blockLevel = 0;

    while (std::getline(cpt.file, cpt.line)) {
        cpt.lineNumber++;
        ws::skip_chars(cpt.line, ws::WHITE_SPACES); // deletes first whitespaces of the line.
        if (cpt.line.empty())
            continue ;
        if (ws::checkClosingBracket(cpt)) {
            if (cpt.blockLevel == 2)
                server = NULL;
            else if (cpt.blockLevel == 3)
                location = NULL;
            std::cout << (cpt.blockLevel == 3 ? "\t" : "");
            std::cout << "closing bracket" << std::endl;
            cpt.blockLevel--;
            continue ;
        }

        lineType = getBlockType(cpt, cpt.line);
        switch (cpt.blockLevel) {
			case 0: // check if we are inside the http, else it's dead
				if (lineType != HTTP)
                    throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", only http blocks can be found at this level.");
				std::cout << "http line: " << cpt.line <<std::endl;
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
					std::cout << cpt.line << std::endl;

                    if (server->getServerInfo().getLocations().count(cpt.line) != 0)
                        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", location block \"" + cpt.line + "\" already declared.");

                    location = new Location();
                    server->getServerInfo().addLocation(cpt.line, location);
                    cpt.blockLevel++;
                } else if (lineType == INFO) {
					if (parseServerLine(cpt, *server) == 1)
						return 1;
				}
                break ;

            case 3: //we are inside location block
                if (lineType != INFO) {
                    throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ": \"" + cpt.line +
                                                "\" is invalid. There can't be a location block inside another location block.");
				}
                if (parseLocationLine(cpt, *location) == 1)
					return 1;
                break ;

            default:
				std::cout << "0 - Error Value" << std::endl;
				return 1;
				break;
        }
    }
    if (cpt.blockLevel != 0)
        throw std::invalid_argument("Missing closing bracket!");
    std::cout << "whole file is parsed" << std::endl;
   return 0;
}

bool ws::isPathExist(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

void ws::checkConfiguration(Server *servers) {
		ServerInfo &serverInfo = servers->getServerInfo();
		std::map<std::string, std::string> cgi;
		std::vector<std::string> method;
		std::vector<std::string> locMethod;
		std::map<int, std::string> errorPage;
		std::map<std::string, Location*> locations = serverInfo.getLocations();
		Location defaultLocation;

		method = serverInfo.getMethod();
		errorPage = serverInfo.getError();
		cgi = serverInfo.getCgis();



		std::cout << "\n----------------------------------SETUP SERVER----------------------------------" << std::endl;
		std::cout << "IP: \t\t\t" << serverInfo.getIp() << std::endl;

		std::cout << "PORT: \t\t\t" << serverInfo.getPort() << std::endl;

		std::cout << "NAME: \t\t\t" << serverInfo.getServerName() << std::endl;

		std::cout << "ROOT: \t\t\t" << serverInfo.getRootPath() << std::endl;

		std::cout << "CLIENT_MAX_SIZE_BODY:\t" << serverInfo.getMaxBodySize() << std::endl;

		std::cout << "INDEX: \t\t\t" << (serverInfo.getIndexPath()) << std::endl;

		if (serverInfo.getUploadPath().empty())
			std::cout << "\t[Upload is Empty]" << std::endl;


		std::cout << "UPLOAD: \t\t" << (serverInfo.getUploadPath()) << std::endl;

		std::cout << "AUTOINDEX: \t\t" << serverInfo.hasAutoindex() << std::endl;
		defaultLocation.setRootPath(serverInfo.getRootPath());
		defaultLocation.setIndexPath(serverInfo.getIndexPath());
		defaultLocation.setUploadPath(serverInfo.getUploadPath());
		defaultLocation.setAutoIndex(serverInfo.hasAutoindex());

		std::cout << "METHOD: \t\t" << std::endl;
		for(std::vector<std::string>::const_iterator it = method.begin(); it != method.end(); ++it) {
			std::cout << "\t\t\t- " << *it << "\n";
			defaultLocation.addtoMethod(*it);
		}
		serverInfo.setDefaultLocation(defaultLocation);

		std::cout << "CGIS: " << std::endl;
		for(std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it) {
			std::cout << "\t\t\t- " << it->first << " \t" << it->second << "\n";
		}

		std::cout << "ERROR_PAGE" << std::endl;
		for(std::map<int, std::string>::iterator it = errorPage.begin(); it != errorPage.end(); ++it) {
			std::cout << "\t\t\t- " << it->first << " \t" << (it->second) << "\n";
		}

		std::cout << "LOCATION: " << std::endl;
		for(std::map<std::string, Location *>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
			Location *loc = it->second;
			locMethod = loc->getMethod();
			std::cout << "\n\t" << it->first << std::endl;

			if (loc->getIndexPath().empty())
				loc->setIndexPath(serverInfo.getIndexPath());
			std::cout << "\t\tINDEX: \t\t" << loc->getIndexPath() << std::endl;

			if (loc->getAutoindex() == -1)
				loc->setAutoIndex(serverInfo.hasAutoindex());
			std::cout << "\t\tAUTOINDEX: \t" << loc->getAutoindex() << std::endl;

			if (loc->getRootPath().empty())
				loc->setRootPath(serverInfo.getRootPath());
			std::cout << "\t\tROOT: \t\t" <<  loc->getRootPath()<< std::endl;

			if (loc->getUploadPath().empty())
				loc->setUploadPath(serverInfo.getUploadPath());
			std::cout << "\t\tUPLOAD: \t" << serverInfo.getUploadPath() << std::endl;

			if (locMethod.empty())
				locMethod = serverInfo.getMethod();
			std::cout << "\t\tMETHOD: " << std::endl;
			for(std::vector<std::string>::const_iterator it = locMethod.begin(); it != locMethod.end(); ++it) {
				std::cout << "\t\t\t\t- " << *it << "\n";
			}
		}
		std::cout << serverInfo.getLocations().at("/www/html/test")->getRootPath() << std::endl;
		std::cout << "\n----------------------------------END OF SETUP----------------------------------\n" << std::endl;
}
