#ifndef SERVERCONFIGROUTES_HPP
# define SERVERCONFIGROUTES_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <sys/stat.h>
#include <map>
#include <vector>

class ServerConfigRoutes
{
	public:
	
	// Setters
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
		void setConfigMethods(const std::string &methods)
		{
			_configMethods.clear();
			std::string method;
			for (size_t i = 0; i < methods.size(); ++i)
			{
				if (methods[i] == ' ')
				{
					if (!method.empty())
						_configMethods.push_back(method);
					method.clear();
				}
				else
					method += methods[i];
			}
			if (!method.empty())
				_configMethods.push_back(method);
		}
		void setCGIPath(const std::string &path) {_cgi_path = path;}
		void setCGIExt(const std::string &extention) {_cgi_ext = extention;}
		void setRedirection(const std::string &redirection)
		{
			this->_redirection[atoi(redirection.substr(0, redirection.find(' ')).c_str())] = redirection.substr(redirection.find(' ') + 1);
		}
		void setUpload(const std::string &upload)
		{
			char *end;
   			long temp = std::strtol(upload.c_str(), &end, 10);
    		if (*end != '\0') {
        		_upload = -1;
    		}
    		_upload = static_cast<int8_t>(temp);
		}
		void setUploadDirectory(const std::string &upload_directory) {_uploadDirectory = upload_directory;}
		void setRouteLoc(const std::string &route_loc) {_routeLoc = route_loc;}
		void setRootPath(const std::string &root_path) {_rootPath = root_path;}

	// Getters
		const std::string& getRouteLoc() const { return _routeLoc; }
    	const std::vector<std::string>& getConfigMethods() const { return _configMethods; }
    	const std::string& getRootPath() const { return _rootPath; }
    	signed char getAutoIndex() const { return _autoIndex; }
    	const std::string& getDefaultFile() const { return _defaultFile; }
    	const std::string& getUploadDirectory() const { return _uploadDirectory; }
    	signed char getUpload() const { return _upload; }
    	const std::string& getCgiPath() const { return _cgi_path; }
    	const std::string& getCgiExt() const { return _cgi_ext; }
    	const std::map<int, std::string>& getRedirection() const { return _redirection; }

		typedef void (ServerConfigRoutes::*Setter)(const std::string&);
        void assign(const std::string &key, const std::string &value);

		ServerConfigRoutes(std::istream& stream);
		void verify_validity();
	
	private:
		std::string _routeLoc;
		std::vector<std::string> _configMethods;
		std::string _rootPath;
		signed char _autoIndex;
		std::string _defaultFile;
		std::string _uploadDirectory;
		signed char _upload;
		std::string _cgi_path;
        std::string _cgi_ext;
		std::map<int, std::string> _redirection;

		// Map for assign()
        static std::map<std::string, Setter> initMap();
        static const std::map<std::string, Setter> _setters;

};

#endif