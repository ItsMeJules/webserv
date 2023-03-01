#ifndef	LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>
# include <map>

class utils;
class ServerInfo;

class Location {
	private:
		std::string _indexPath;
		std::string _rootPath;
		std::string _uploadPath;

		int _autoIndex;

		std::vector<std::string> _method;
	public:
		Location();
		Location(Location const &location);
		~Location();

		Location &operator=(Location const &rhs);

		const std::string &getIndexPath() const;
		const std::string &getRootPath() const;
		const std::string &getUploadPath() const;
		const int &getAutoindex() const;
		const std::vector<std::string> &getMethod() const;

		void  setIndexPath(std::string Path);
		void  setRootPath(std::string Path);
		void  setUploadPath(std::string Path);
		void  setAutoIndex(int AutoIndex);
		void  addtoMethod(std::string Method);

		static const Location &getBestMatch(std::string const &url, const ServerInfo &serverInfo);
};

# include "utils.hpp"
# include "ServerInfo.hpp"

#endif
