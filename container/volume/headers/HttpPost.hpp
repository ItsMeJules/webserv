#ifndef HTTPPOST_HPP
# define HTTPPOST_HPP

# include <fstream>

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

# include "HttpMethod.hpp"
# include "AMessageBody.hpp"
# include "FormDataBody.hpp"

class HttpPost : public HttpMethod {
	private:
		bool writePartToFile(FormDataBody::FormDataPart &part, std::string const &filePath, std::ofstream &ofs);
	public:
		HttpPost();
		HttpPost(HttpPost const &httpPost);
		~HttpPost();

		HttpResponse execute(ServerInfo const &info, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();
		
		HttpPost &operator=(HttpPost const &rhs);
};

#endif