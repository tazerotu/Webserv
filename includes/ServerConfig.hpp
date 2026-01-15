#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <errno.h>
#include <sys/stat.h>
#include <map>
#include <vector>
#include "ServerConfigRoutes.hpp"

class ServerConfig
{

    public:
        typedef void (ServerConfig::*Setter)(const std::string&);

        // Constructor / Destructor
        ServerConfig(std::string conf);
		ServerConfig(std::istream& stream);
        ~ServerConfig();

        // Assign from key/value
        void assign(const std::string &key, const std::string &value);

        // Getters
        const std::string &getName() const;
        const std::string &getHost() const;
        int getPort() const;
        const std::string &getRoot() const;
        int getMaxClientBodySize() const;
        const std::map<std::string, std::string> &getErrorPages() const;
		std::vector<ServerConfigRoutes> getRoutes() const;
        // const std::string &getIndex() const;
        // int getAutoindex() const;
        // const std::vector<std::string> &getAllowedMethods() const;
        // const std::string &getCgiPath() const;
        // const std::string &getCgiExt() const;

        // Setters
        void setName(const std::string &name);
        void setHost(const std::string &host);
        void setPort(const std::string &port);
        void setRoot(const std::string &root);
        void setErrorPage(const std::string &errorPage);
        void setMaxClientBodySize(const std::string &size);
        // void setIndex(const std::string &index);
        // void setAutoindex(const std::string &autoindex);
        // void setAllowedMethods(const std::string &methods);
        // void setCgiPath(const std::string &cgiPath);
        // void setCgiExt(const std::string &cgiExt);

        void verify_validity();

    private:
        // Attributes
        std::string _name;
        std::string _host;
        int         _port;
        std::string _root;
        std::map<std::string, std::string> _error_pages;
        int         _maxClientBodySize;
		std::vector<ServerConfigRoutes> _routes;
        // std::string _index;
        // int         _autoindex;
        // std::vector<std::string> _allowed_methods;
        // std::string _cgi_path;
        // std::string _cgi_ext;

        // Map for assign()
        static std::map<std::string, Setter> initMap();
        static const std::map<std::string, Setter> _setters;
};

#endif