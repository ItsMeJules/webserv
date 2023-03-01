#ifndef FORMDATABODY_HPP
# define FORMDATABODY_HPP

# include "AMessageBody.hpp"

# include <map>

class FormDataBody : public AMessageBody {
	public:
		class FormDataPart {
			public:
				FormDataPart();
				FormDataPart(FormDataPart const &formDataPart);
				~FormDataPart();

				std::map<std::string, std::string> _headers;
				std::map<std::string, std::vector<char> > _directives;
				
				std::string _directiveName;
				std::string _fileKey;
				std::string _fileName;
				std::vector<char> _contents;

				bool _headersParsed;
				bool _bodyParsed;
				
				void parseHeaders();
				void parseBody(FormDataBody &parent);
				std::string extractBody();

				FormDataPart &operator=(FormDataPart const &rhs);
		};
	private:
		std::vector<FormDataPart*> _parts;
		std::vector<char> _tmp;
		std::string _boundary;

		FormDataPart &getNextNeedParsing();
	public:
		FormDataBody();
		FormDataBody(ADataDecoder *decoder, std::string boundaryHeader);
		FormDataBody(FormDataBody const &formDataBody);
		~FormDataBody();

        int parse(char *body, int &size);
		AMessageBody *clone();
		FormDataBody::FormDataPart *readForm();

		std::vector<FormDataPart*> getDataParts();
		FormDataPart *getFilePart();
		std::string getBodyStr();

		FormDataBody &operator=(FormDataBody const &rhs);
};

#endif