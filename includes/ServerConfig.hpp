#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <errno.h>

class ServerConfig
{
	private:
	// Attributes
	std::string _name;
	std::string _host;
	int			_port;
	std::string _root;
	std::string _index;
	std::string _error_page;
	std::string *_allowed_methods;
	int			_maxClientBodySize;
	std::string _cgi_path;
	std::string _cgi_ext;

	public:
	// Constructors
	ServerConfig();
	ServerConfig(std::string conf);

	// Destructors
	~ServerConfig();

	// Methods
	std::string const &getName(void) const;
	std::string const &getHost(void) const;
	int                getPort(void) const;
	std::string const &getRoot(void) const;
	std::string const &getIndex(void) const;
	std::string const &getErrorPage(void) const;
	std::string const *getAllowedMethods(void) const;
	int                getMaxClientBodySize(void) const;
	std::string const &getCgiPath(void) const;
	std::string const &getCgiExt(void) const;
};

#endif