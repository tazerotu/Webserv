#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <errno.h>
#include <sys/stat.h>
#include <map>

class ServerConfig
{
	private:
	// Attributes
	std::string _name;
	std::string _host;
	int			_port;
	std::string _root;
	std::string _index;
	int			_autoindex;
	std::string *_error_page;
	std::string *_allowed_methods;
	int			_maxClientBodySize;
	std::string _cgi_path;
	std::string _cgi_ext;

	public:
	// Constructor
	ServerConfig(std::string conf);

	// Destructor
	~ServerConfig();

	// Getters
	const std::string &getName() const;
	const std::string &getHost() const;
	int getPort() const;
	const std::string &getRoot() const;
	const std::string &getIndex() const;
	int getAutoindex() const;
	std::string *getErrorPage() const;
	std::string *getAllowedMethods() const;
	int getMaxClientBodySize() const;
	const std::string &getCgiPath() const;
	const std::string &getCgiExt() const;

	// Setters 
	void setName(const std::string &name);
	void setHost(const std::string &host);
	void setPort(const std::string port);
	void setRoot(const std::string &root);
	void setIndex(const std::string &index);
	void setAutoindex(const std::string autoindex);
	void setErrorPage(std::string *errorPage);
	void setAllowedMethods(std::string *methods);
	void setMaxClientBodySize(const std::string size);
	void setCgiPath(const std::string &cgiPath);
	void setCgiExt(const std::string &cgiExt);

	
};

#endif