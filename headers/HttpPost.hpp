#ifndef HTTPPOST_HPP
# define HTTPPOST_HPP

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

# include "HttpMethod.hpp"
# include "IMessageBody.hpp"

class HttpPost : public HttpMethod {
	private:
		typedef struct file_post_s {
			std::string fileHeader;
			std::string fileName;
			std::string filePath;
			std::string boundary;
		} file_post_t;

		file_post_t _fileInfo;

		const bool isFile(HttpRequest const &request) const;
		const bool fileExists() const;
		void getFileInfo(std::string const &header, std::string const &body);
		const bool createFile(std::string const &path, const IMessageBody *const body);
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