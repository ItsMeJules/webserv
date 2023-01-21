#ifndef HTTPPOST_HPP
# define HTTPPOST_HPP

# include "HttpMethod.hpp"
# include "FileBody.hpp"

class HttpPost : public HttpMethod {
	private:
		typedef struct file_post_s {
			std::string fileHeader;
			std::string fileName;
			std::string filePath;
			std::string boundary;
		} file_post_t;

		file_post_t _fileInfo;

		bool isFile(HttpRequest const &request) const;
		void initFile(std::string const &header, std::string const &body);
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