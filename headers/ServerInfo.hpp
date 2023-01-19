#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include <string>
# include <map>
# include <stdint.h>
# include <vector>

class ServerInfo {
	public:
		class Location {
			private:
				std::string _indexPath;

				bool _autoindex;
			public:
				Location();
				Location(Location const &location);
				~Location();
		
				Location &operator=(Location const &rhs);

                const std::string &getIndexPath() const;
                bool hasAutoindex() const;
        };
	private:
		uint32_t _maxBodySize;
		
		bool _autoindex;

		std::string _serverName;
		std::string _rootPath;
		std::string _indexPath;
		
		std::vector<std::string> _method;
		std::map<std::string, std::string> _cgis;
		std::map<std::string, ServerInfo::Location*> _locations;
		std::map<int, std::string> _errorPage;

	public:
		ServerInfo();
		ServerInfo(ServerInfo const &serverInfo);
		~ServerInfo();

        void addLocation(std::string path, ServerInfo::Location *location);

		const uint32_t &getMaxBodySize() const;
        bool hasAutoindex() const;
		const std::string &getServerName() const;
		const std::string &getRootPath() const;
		const std::string &getIndexPath() const;
		const std::vector<std::string> &getMethod() const;
		const std::map<std::string, std::string> &getCgis() const;
		const std::map<std::string, ServerInfo::Location*> &getLocations() const;
		const std::map<int, std::string> &getError() const;

		void  setMaxBodySize(uint32_t MaxBodySize);
		void  setAutoIndex(bool AutoIndex);
		void  setServerName(std::string ServerName);
		void  setIndexPath(std::string path);
		void  addtoMethod(std::string method);
		void  addToCGIS(std::string extension, std::string path);
		void  addErrorPage(int key, std::string value);

        ServerInfo &operator=(ServerInfo const &rhs);
};
#endif