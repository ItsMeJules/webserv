#ifndef FILEBODY_HPP
# define FILEBODY_HPP

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

# include <vector>

# include "IMessageBody.hpp"

class FileBody : public IMessageBody {
	private:
		typedef struct s_form_data {
			std::string name;
			std::string data;
		} t_form_data;
	
		IMessageBody *_parsingType;

		std::string _fileName;
		std::string _filePath;
		std::string _boundary;

		std::vector<t_form_data> _values;
	public:
		FileBody();
		FileBody(IMessageBody *parsingType, std::string boundaryHeader);
		FileBody(FileBody const &fileBody);
		~FileBody();

		void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive, int const &bodySize);
		void truncBody(int pos, int npos);
		const bool fileExists() const;
		const bool createFile(std::string const &path);

		const std::string &getBody() const;
		const int &getSize() const;
		const std::string &getFileName() const;
		const std::string &getBoundary() const;
		const std::string &getFilePath() const;

		void setFilePath(std::string filePath);
		void setIsFileBody(bool fileBody);

		FileBody &operator=(FileBody const &rhs);
};

#endif