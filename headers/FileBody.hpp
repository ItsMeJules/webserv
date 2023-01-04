#ifndef FILEBODY_HPP
# define FILEBODY_HPP

# include "IMessageBody.hpp"

class FileBody : public IMessageBody {
    private:
		std::stringstream _contents;
		std::string _fileName;
		int _size;
    public:
        FileBody();
        FileBody(FileBody const &fileBody);
        ~FileBody();

        void append(std::string str);
        void append(std::string str, int size);
//        void parse(std::string body);

        std::string getBody() const;
        int getSize() const;
		std::string getFileName() const;

        FileBody &operator=(FileBody const &rhs);
};

#endif