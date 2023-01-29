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
		locationInfo.setAutoIndex(true);
		return (0);
	}
	else if (index.compare("off") == 0) {
		locationInfo.setAutoIndex(false);
		return (0);
	}
	else {
		throw std::invalid_argument(index + "need to ne params by 'on' or 'off' !");
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
	if (p_tmp < 0 && p_tmp > 65535)
		throw std::invalid_argument("Error, Invalid Listen : Port can only be between 0 and 65535.");
	return 0;
}

int ws::checkErrorKey(std::string key) {
	const char *tmp = key.c_str();
	int p_tmp = atoi(tmp);
	int i = 0;

	while (tmp[i])
	{
		if (tmp[i] >= '0' && tmp[i] <= '9')
			i++;
		else
			throw std::invalid_argument("Error, Page_Error : Can only be Number.");
	}
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
	matchValues["rewrite"] = REWRITE;
}

bool	ws::ft_in_charset(char const c, const std::string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}

std::vector<std::string> ws::splitStr(const std::string &str, const std::string &charset)
{
	std::vector<std::string> res;
	std::string			tmp;
	size_t			i;

	i = 0;
	while (i < str.length())
	{
		while (i < str.length() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.length())
		{
			tmp = "";
			while (i < str.length() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return res;
}

void ws::checkPath(std::string const &path) {
	const char *dir = path.c_str();
	struct stat sb;

	if (stat(dir, &sb) != 0)
		throw std::invalid_argument("The path " + path + " doesn't exists.");
}

int ws::parse_server_line(config_parsing_t &cpt, Server &server) {
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
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");

			if (ws::string_in_range(lineArguments[1], ws::AL_NUM, sizeArgumentOne))
				throw std::invalid_argument(lineArguments[1] + " is not a valid name for a server. Only alnum chars are accepted!");

			serverInfo.setServerName(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in ServerName: " << serverInfo.getServerName() << std::endl;
			break;

		case LISTEN: 
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");

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
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");
			
			if (checkClientMaxBodySize(lineArguments[1].substr(0, sizeArgumentOne), serverInfo) != 0)
				throw std::invalid_argument(lineArguments[1].substr(0, sizeArgumentOne) + "must finish with a 'G', 'M' or a 'K' !");

			std::cout << "\tSet in ServerClientBodySizeMax: " << serverInfo.getMaxBodySize() << std::endl;
			break;

		case AUTOINDEX:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");

			if (checkAutoIndex(lineArguments[1].substr(0, sizeArgumentOne), serverInfo) != 0)
				throw std::invalid_argument(lineArguments[1].substr(0, sizeArgumentOne) + "need to be params by 'on' or 'off' !");

			std::cout << "\tSet in ServerAutoIndex: " << serverInfo.hasAutoindex() << std::endl;
			break;

		case INDEX:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");
			checkPath(lineArguments[1].substr(0, sizeArgumentOne));
			serverInfo.setIndexPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in ServerAutoIndex: " << serverInfo.getIndexPath() << std::endl;
			break;

		case UPLOAD:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");
			checkPath(lineArguments[1].substr(0, sizeArgumentOne));
			serverInfo.setUploadPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in ServerUploadPath: " << serverInfo.getUploadPath() << std::endl;
			break;

		case ROOT:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");
			checkPath(lineArguments[1].substr(0, sizeArgumentOne));
			serverInfo.setRootPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in ServerRootPath: " << serverInfo.getRootPath() << std::endl;
			break;

		case CGI:
			if (lineArguments.size() != 3)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[2][lineArguments[2].size() - 1] != ';')
				throw std::invalid_argument(lineArguments[2] + " must finish with a ';'!");
			
			checkPath(lineArguments[2].substr(0, lineArguments[2].size() - 1));

			serverInfo.addToCGIS(lineArguments[1], lineArguments[2].substr(0, lineArguments[2].size() - 1));
			cgi = serverInfo.getCgis();
			for(std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it)
			{
				std::cout << "\tSet in ServerCGI : First: " << it->first << " Second: " << it->second << "\n";
			}
			break;
		
		case METHOD:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[2] + " must finish with a ';'!");
			
			values = splitStr(lineArguments[1].substr(0, sizeArgumentOne), "|");
			for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (*it != "GET" && *it != "POST" && *it != "DELETE")
					throw std::invalid_argument(*it + " need to be params by 'GET', 'POST' or 'DELETE' !");
				serverInfo.addtoMethod(*it);
			}
			method = serverInfo.getMethod();
			for(std::vector<std::string>::const_iterator it = method.begin(); it != method.end(); ++it)
			{
				std::cout << "\tSet in ServerMethod: " << *it << "\n";
			}
			break;

		case ERROR_PAGE:
			if (lineArguments.size() != 3)
				throw std::length_error("Wrong number of arguments, 3 expected.");

			if (lineArguments[2][lineArguments[2].size() - 1] != ';')
				throw std::invalid_argument(lineArguments[2] + " must finish with a ';'!");

			tmp = ws::stoi(lineArguments[1]);
			if (HttpResponse::codes.count(tmp) == 0)
				throw std::invalid_argument("Error, Page_Error : The Error Key doen't exist.");
			
			checkPath(lineArguments[2].substr(0, lineArguments[2].size() - 1));

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

int ws::checkMethod(std::string method, Location &locationInfo) {
	if ((method.compare("GET") == 0) || (method.compare("POST") == 0) || (method.compare("DELETE") == 0)) {
		locationInfo.addtoMethod(method);
		return (0);
	}
	else {
		std::cerr << "Problem Configuration Files - METHOD" << std::endl;
		return (1);
	}
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
	
	switch(cpt.configKeys[lineArguments[0]]) {
		case INDEX:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");
			
			checkPath(lineArguments[1].substr(0, sizeArgumentOne));
			location.setIndexPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in LocationIndex: " << location.getIndexPath() << std::endl;
			break;
		
		case AUTOINDEX:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");

			if (checkLocationAutoIndex(lineArguments[1].substr(0, sizeArgumentOne), location) != 0)
				throw std::invalid_argument(lineArguments[1].substr(0, sizeArgumentOne) + "need to be params by 'on' or 'off' !");
			std::cout << "\tSet in LocationAutoIndex: " << location.hasAutoindex() << std::endl;
			break;

		case METHOD:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[2] + " must finish with a ';'!");
			
			values = splitStr(lineArguments[1].substr(0, sizeArgumentOne), "|");
			for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (*it != "GET" && *it != "POST" && *it != "DELETE")
					throw std::invalid_argument(*it + " need to be params by 'GET', 'POST' or 'DELETE' !");
				location.addtoMethod(*it);
			}
			method = location.getMethod();
			for(std::vector<std::string>::const_iterator it = method.begin(); it != method.end(); ++it)
			{
				std::cout << "\tSet in LocationMethod: " << *it << "\n";
			}
			break;

		case UPLOAD:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");

			checkPath(lineArguments[1].substr(0, sizeArgumentOne));
			location.setUploadPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in LocationUpload: " << location.getUploadPath() << std::endl;
			break;

		case REWRITE:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");
			
			checkPath(lineArguments[1].substr(0, sizeArgumentOne));
			location.setRewritePath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in LocationRewrite: " << location.getRewritePath() << std::endl;
			break;

		case ROOT:
			if (lineArguments.size() != 2)
				throw std::length_error("Wrong number of arguments, 2 expected.");

			if (lineArguments[1][sizeArgumentOne] != ';')
				throw std::invalid_argument(lineArguments[1] + " must finish with a ';'!");
			
			checkPath(lineArguments[1].substr(0, sizeArgumentOne));
			location.setRootPath(lineArguments[1].substr(0, sizeArgumentOne));
			std::cout << "\tSet in LocationRoot: " << location.getRootPath() << std::endl;
			break;
		
		default:
			throw std::invalid_argument("Default Error, the word " + lineArguments[0] + " doesn't exist.");
			break;

	}

}

// int ws::parse_location_line(config_parsing_t &cpt, Location &location) {
//     std::cout << "\t\t\tlocation line: " << cpt.line << std::endl;
//     char *cstr = new char [cpt.line.length()+1];
//     strcpy (cstr, cpt.line.c_str());
//     char *p = ::strtok (cstr, " ");
// 	std::string str = std::string(p);

// 	std::map<std::string, confValues> matchValues;
// 	parserInit(matchValues);

// 	Location loc = location;
// 	switch (matchValues[str])
// 	{
// 		case INDEX:
// 			p = strtok(NULL, " ,|;");
// 			while (p!=0)
// 			{
// 				location.setIndexPath(p);
// 				p = strtok(NULL," ,|;");
// 			}
// 			break;

// 		case AUTOINDEX:
// 			p = strtok(NULL," ,|;");
// 			while (p!=0) {
// 				std::string index = std::string(p);
// 				if (checkAutoIndex(index, location))
// 					return (1);
// 				p = strtok(NULL," ,|;");
// 			}
// 			break;

// 		case METHOD:
// 			p = strtok(NULL," ,|;");
// 			while (p!=0) {
// 				std::string method = std::string(p);
// 				if (checkMethod(method, location))
// 					return (1);
// 				p = strtok(NULL," ,|;");
// 			}
// 			break;

// 		case UPLOAD:
// 			p = strtok(NULL," ,|;");
// 			while (p!=0)
// 			{
// 				location.setUploadPath(p);
// 				p = strtok(NULL," ,|;");
// 			}
// 			break;

// 		case REWRITE:
// 			p = strtok(NULL," ,|;");
// 			while (p!=0)
// 			{
// 				location.setRewritePath(p);
// 				p = strtok(NULL," ,|;");
// 			}
// 			break;

// 		case ROOT:
// 			p = strtok(NULL," ,|;");
// 			while (p!=0)
// 			{
// 				location.setRootPath(p);
// 				p = strtok(NULL," ,|;");
// 			}
// 			break;

// 		default:
// 			std::cout << "0 - Error Value" << std::endl;
// 			return (1);
// 			break;
// 	}
// 	// std::cout << "\tIndex: " << location.getIndexPath() << std::endl;
// 	// std::cout << "\tAutoIndex: " << location.hasAutoindex() << std::endl;
// 	// std::cout << "\tRewrite: " << location.getRewritePath() << std::endl;
// 	// std::cout << "\tRoot: " << location.getRootPath() << std::endl;
// 	// std::cout << "\tUpload: " << location.getUploadPath() << std::endl;
// 	// std::vector<std::string> test = location.getMethod();
// 	// std::cout << "\tMethod: " << " \t";
// 	// for (std::vector<std::string>::const_iterator i = test.begin(); i != test.end(); ++i)
//     // 	std::cout << *i << ' ';
// 	// std::cout << std::endl;
// 	delete[] cstr;
// 	return 0;
// }

int ws::parse_config(std::string const &name, std::vector<Server*> &servers) {
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
                    //ws::checkPath(cpt.line);

                    if (server->getServerInfo().getLocations().count(cpt.line) != 0)
                        throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ", location block \"" + cpt.line + "\" already declared.");

                    location = new Location();
                    server->getServerInfo().addLocation(cpt.line, location);
                    cpt.blockLevel++;
                } else if (lineType == INFO) {
					if (parse_server_line(cpt, *server) == 1)
						return 1;
				}     
                break ;

            case 3: //we are inside location block
                if (lineType != INFO)
                    throw std::invalid_argument("Error on line " + ws::itos(cpt.lineNumber) + ": \"" + cpt.line +
                                                "\" is invalid. There can't be a location block inside another location block.");
                    if(parseLocationLine(cpt, *location) == 1)
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