#ifndef FILEBODY_HPP
# define FILEBODY_HPP

# include "IMessageBody.hpp"

class FileBody : public IMessageBody {
    private:
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
//        void parse(std::string body);

        std::string getBody() const;
        std::string getFileHeader() const;
		std::string getFileName() const;
        std::string getBoundary() const;
        int getSize() const;

        void parseFileHeader(std::string body);
        void setBoundary(std::string header);

        FileBody &operator=(FileBody const &rhs);
};

#endif