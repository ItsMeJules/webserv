#ifndef FORMDATABODY_HPP
# define FORMDATABODY_HPP

# include "AMessageBody.hpp"

# include <map>

class FormDataBody : public AMessageBody {
	private:
		class FormDataPart {
			private:
				int strPos(std::string str) const;
			public:
				FormDataPart();
				FormDataPart(FormDataPart const &formDataPart);
				~FormDataPart();

				std::map<std::string, std::string> _headers;
				std::map<std::string, std::vector<char> > _directives;
				
				std::string _fileKey;
				std::string _fileName;
				std::vector<char> _contents;

				bool _parsed;
				
				bool parse(FormDataBody const &parent, size_t const &partEndPos);

				FormDataPart &operator=(FormDataPart const &rhs);
		};

		std::vector<FormDataPart*> _parts;
		std::vector<char> _tmp;
		std::string _boundary;

		int nextCRLFpos(int pos = 0, int nCRLF = 1) const;
	public:
		FormDataBody();
		FormDataBody(ADataDecoder *decoder, std::string boundaryHeader);
		FormDataBody(FormDataBody const &formDataBody);
		~FormDataBody();

        int parse(char *body, int &size);

		std::string getBodyStr();

		FormDataBody &operator=(FormDataBody const &rhs);
};

#endif