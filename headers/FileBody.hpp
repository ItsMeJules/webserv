#ifndef FILEBODY_HPP
# define FILEBODY_HPP

# include "utils.hpp"

class FileBody : public IMessageBody {
    private:
        std::stringstream _inReceive;
		std::stringstream _contents;
        std::string _fileHeader;
		std::string _fileName;
        std::string _boundary;

		int _size;
    public:
        FileBody();
        FileBody(FileBody const &fileBody);
        ~FileBody();

        void append(std::string str);
        void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive);

        const std::string getBody() const;
        const std::string &getFileHeader() const;
		const std::string &getFileName() const;
        const std::string &getBoundary() const;
        const int &getSize() const;

        void setBoundary(std::string header);

        FileBody &operator=(FileBody const &rhs);
};

#endif
