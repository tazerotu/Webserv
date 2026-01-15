#ifndef SERVERCONFIGROUTES_HPP
# define SERVERCONFIGROUTES_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <errno.h>
#include <sys/stat.h>
#include <map>
#include <vector>

class ServerConfigRoutes
{
	public:
		void setDefaultFile(const std::string &file) {_defaultFile = file;}
		void setAutoIndex(const std::string &value) 
		{
			char *end;
   			long temp = std::strtol(value.c_str(), &end, 10);
    		if (*end != '\0') {
        		_autoIndex = -1;
    		}
    		_autoIndex = static_cast<int8_t>(temp);
		}
		void setConfigMethods(const std::string &methods);
		void setCGIPath(const std::string &path) {_cgi_path = path;}
		void setCGIExt(const std::string &extention) {_cgi_ext = extention;}
		void setRedirection(const std::string &redirection);
		void setUpload(const std::string &upload)
		{
			char *end;
   			long temp = std::strtol(upload.c_str(), &end, 10);
    		if (*end != '\0') {
        		_upload = -1;
    		}
    		_upload = static_cast<int8_t>(temp);
		}
		void setUploadDirectory(const std::string &upload_directory);
		void setRouteLoc(const std::string &route_loc) {_routeLoc = route_loc;}
		void setRootPath(const std::string &root_path) {_rootPath = root_path;}

		typedef void (ServerConfigRoutes::*Setter)(const std::string&);
        void assign(const std::string &key, const std::string &value);

		ServerConfigRoutes(std::istream& stream);
		void verify_validity();
	
	private:
		std::string _routeLoc;
		std::vector<std::string> _configMethods;
		std::string _rootPath;
		int8_t _autoIndex;
		std::string _defaultFile;
		std::string _uploadDirectory;
		int8_t _upload;
		std::string _cgi_path;
        std::string _cgi_ext;
		std::map<int, std::string> _redirection;

		// Map for assign()
        static std::map<std::string, Setter> initMap();
        static const std::map<std::string, Setter> _setters;

};

#endif