#pragma
#ifndef PARSINGROUTES_HPP
# define PARSINGROUTES_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <sys/stat.h>
#include <map>
#include <vector>


/**
 * @brief Handles the routes for the Parsing class of the Webserv project
 */
class ParsingRoutes
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
			if (redirection.empty())
				return;

			std::string::size_type space = redirection.find(' ');
			if (space == std::string::npos)
				return;

			std::string codeStr = redirection.substr(0, space);
			std::string target  = redirection.substr(space + 1);

			if (codeStr.empty() || target.empty())
				return;

			char *end;
			long code = std::strtol(codeStr.c_str(), &end, 10);
			if (*end != '\0')
				return;

			_redirection[(int)code] = target;
		}
		void setUpload(const std::string &upload)
		{
			char *end;
   			long temp = std::strtol(upload.c_str(), &end, 10);
    		if (*end != '\0') {
        		_upload_status = -1;
    		}
    		_upload_status = static_cast<int8_t>(temp);
		}
		void setRouteLoc(const std::string &route_loc) {_routeLoc = route_loc;}
		void setRootPath(const std::string &root_path) {_rootPath = root_path;}

	// Getters
		const std::string& getRouteLoc() const { return _routeLoc; }
    	const std::vector<std::string>& getConfigMethods() const { return _configMethods; }
    	const std::string& getRootPath() const { return _rootPath; }
    	signed char getAutoIndex() const { return _autoIndex; }
    	const std::string& getDefaultFile() const { return _defaultFile; }
    	signed char getUpload() const { return _upload_status; }
    	const std::string& getCgiPath() const { return _cgi_path; }
    	const std::string& getCgiExt() const { return _cgi_ext; }
    	const std::map<int, std::string>& getRedirection() const { return _redirection; }

		typedef void (ParsingRoutes::*Setter)(const std::string&);
        void assign(const std::string &key, const std::string &value);

		ParsingRoutes(std::istream& stream);
		void verify_route();
	
	private:
		std::string _routeLoc;
		std::vector<std::string> _configMethods;
		std::string _rootPath;
		signed char _autoIndex;
		std::string _defaultFile;
		signed char _upload_status;
		std::string _cgi_path;
        std::string _cgi_ext;
		std::map<int, std::string> _redirection;

		// Map for assign()
        static std::map<std::string, Setter> initMap();
        static const std::map<std::string, Setter> _setters;

};

#endif