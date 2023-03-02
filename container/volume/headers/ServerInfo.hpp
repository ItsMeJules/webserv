#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include <stdint.h>

# include <string>
# include <map>
# include <vector>

# include "Location.hpp"

class ServerInfo {
	private:
		uint32_t _maxBodySize;

		bool _autoindex;

		std::string _serverName;
		std::string _rootPath;
		std::string _indexPath;
		std::string _uploadPath;
		std::string _ip;
		int			_port;

		Location	_defaultLocation;

		std::vector<std::string> _method;
		std::map<std::string, std::string> _cgis;
		std::map<std::string, Location*> _locations;
		std::map<int, std::string> _errorPage;

	public:
		ServerInfo();
		ServerInfo(ServerInfo const &serverInfo);
		~ServerInfo();

        void addLocation(std::string path, Location *location);

		const uint32_t &getMaxBodySize() const;
        bool hasAutoindex() const;
		const std::string &getServerName() const;
		const std::string &getRootPath() const;
		const std::string &getIndexPath() const;
		const std::string &getUploadPath() const;
		const std::string &getIp() const;

		const int &getPort() const;

		const Location &getDefaultLocation() const;

		const std::vector<std::string> &getMethod() const;
		const std::map<std::string, std::string> &getCgis() const;
		const std::map<std::string, Location*> &getLocations() const;
		std::map<int, std::string> &getError();
		const std::map<int, std::string> &getErrorPages() const;

		void  setMaxBodySize(uint32_t MaxBodySize);
		void  setIp(std::string ip);
		void  setPort(int port);
		void  setAutoIndex(bool AutoIndex);
		void  setServerName(std::string ServerName);
		void  setRootPath(std::string Path);
		void  setIndexPath(std::string Path);
		void  setUploadPath(std::string Path);
		void  addtoMethod(std::string Method);
		void  addToCGIS(std::string Extension, std::string Path);
		void  addErrorPage(int Key, std::string Value);
		void  setLocation(std::map<std::string, Location*> location);
		void  setErrorPage(std::map<int, std::string> error);
		void setDefaultLocation(Location defaultLocation);

        ServerInfo &operator=(ServerInfo const &rhs);
};
#endif
