#ifndef PARSING_HPP
# define PARSING_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <errno.h>
#include <sys/stat.h>
#include <map>
#include <vector>
#include "ParsingRoutes.hpp"

class Parsing
{

    public:
        typedef void (Parsing::*Setter)(const std::string&);

        // Constructor / Destructor
        Parsing(std::string conf);
		Parsing(std::istream& stream);
        ~Parsing();

        // Assign from key/value
        void assign(const std::string &key, const std::string &value);

        // Getters
        const std::string &getName() const;
        const std::string &getHost() const;
        int getPort() const;
        const std::string &getRoot() const;
        int getMaxClientBodySize() const;
        std::map<int, std::string> &getErrorPages();
		std::vector<ParsingRoutes> getRoutes() const {return _routes;}
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
        std::map<int, std::string> _error_pages;
        int         _maxClientBodySize;
		std::vector<ParsingRoutes> _routes;
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