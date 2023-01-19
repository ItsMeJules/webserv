#ifndef FILEBODY_HPP
# define FILEBODY_HPP

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

# include "IMessageBody.hpp"

class FileBody : public IMessageBody {
    private:
		std::stringstream _contents;

        std::string _fileHeader;
		std::string _fileName;
        std::string _boundary;
        std::string _path;

		int _size;
    public:
        FileBody();
        FileBody(FileBody const &fileBody);
        ~FileBody();

        void append(std::string str);
        void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive);
        const bool createFile(std::string const &path);
        const bool fileExists() const;

        const std::string getBody() const;
        const std::string &getFileHeader() const;
		const std::string &getFileName() const;
        const std::string &getBoundary() const;
        const std::string &getPath() const;
        const int &getSize() const;

        void setBoundary(std::string header);

        FileBody &operator=(FileBody const &rhs);
};

#endif