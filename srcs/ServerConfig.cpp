/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:31:12 by ttas              #+#    #+#             */
/*   Updated: 2025/11/19 11:23:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

#include <cstdlib> 

// ---------------- Static Helper ----------------

static void error_message(const std::string &message)
{
    std::cout << message << std::endl;
    exit(1);
}

static bool isRegularFile(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return false;
    return S_ISREG(st.st_mode);
}

// ---------------- Constructor ----------------

ServerConfig::ServerConfig(std::string conf)
{
    if (conf.size() < 5 || conf.substr(conf.size() - 5) != ".conf")
        error_message("Invalid config file extension. Expected .conf");

    if (!isRegularFile(conf))
        error_message("Config path must be a regular file");

    std::ifstream file(conf.c_str());
    if (!file.is_open())
        error_message("Empty or non-existent config file");

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        int pos = line.find_first_of(" ");
        if (pos <= 0)
            error_message("Invalid config line: " + line);

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (key.empty() || value.empty())
            error_message("Key or value is empty in line: " + line);

        assign(key, value);
    }
    file.close();
}

// ---------------- Destructor ----------------

ServerConfig::~ServerConfig()
{
}

// ---------------- Getters ----------------

const std::string &ServerConfig::getName() const { return _name; }
const std::string &ServerConfig::getHost() const { return _host; }
int ServerConfig::getPort() const { return _port; }
const std::string &ServerConfig::getRoot() const { return _root; }
const std::string &ServerConfig::getIndex() const { return _index; }
int ServerConfig::getAutoindex() const { return _autoindex; }
const std::map<std::string, std::string> &ServerConfig::getErrorPages() const { return _error_pages; }
const std::vector<std::string> &ServerConfig::getAllowedMethods() const { return _allowed_methods; }
int ServerConfig::getMaxClientBodySize() const { return _maxClientBodySize; }
const std::string &ServerConfig::getCgiPath() const { return _cgi_path; }
const std::string &ServerConfig::getCgiExt() const { return _cgi_ext; }

// ---------------- Setters ----------------

void ServerConfig::setName(const std::string &name) { _name = name; }
void ServerConfig::setHost(const std::string &host) { _host = host; }
void ServerConfig::setPort(const std::string &port) { _port = atoi(port.c_str()); }
void ServerConfig::setRoot(const std::string &root) { _root = root; }
void ServerConfig::setIndex(const std::string &index) { _index = index; }
void ServerConfig::setAutoindex(const std::string &autoindex) { _autoindex = atoi(autoindex.c_str()); }
void ServerConfig::setErrorPage(const std::string &errorPage)
{
    this->_error_pages[errorPage.substr(0, errorPage.find(' '))] = errorPage.substr(errorPage.find(' ') + 1);
}

void ServerConfig::setAllowedMethods(const std::string &methods)
{
    _allowed_methods.clear();
    std::string method;
    for (size_t i = 0; i < methods.size(); ++i)
    {
        if (methods[i] == ' ')
        {
            if (!method.empty())
                _allowed_methods.push_back(method);
            method.clear();
        }
        else
            method += methods[i];
    }
    if (!method.empty())
        _allowed_methods.push_back(method);
}
void ServerConfig::setMaxClientBodySize(const std::string &size) { _maxClientBodySize = atoi(size.c_str()); }
void ServerConfig::setCgiPath(const std::string &cgiPath) { _cgi_path = cgiPath; }
void ServerConfig::setCgiExt(const std::string &cgiExt) { _cgi_ext = cgiExt; }

// ---------------- Map Initialization ----------------

std::map<std::string, ServerConfig::Setter> ServerConfig::initMap()
{
    std::map<std::string, Setter> m;
    m["name"] = &ServerConfig::setName;
    m["host"] = &ServerConfig::setHost;
    m["port"] = &ServerConfig::setPort;
    m["root"] = &ServerConfig::setRoot;
    m["index"] = &ServerConfig::setIndex;
    m["autoindex"] = &ServerConfig::setAutoindex;
    m["error_page"] = &ServerConfig::setErrorPage;
    m["allowed_methods"] = &ServerConfig::setAllowedMethods;
    m["client_max_body"] = &ServerConfig::setMaxClientBodySize;
    m["cgi_path"] = &ServerConfig::setCgiPath;
    m["cgi_ext"] = &ServerConfig::setCgiExt;
    return m;
}

const std::map<std::string, ServerConfig::Setter> ServerConfig::_setters = ServerConfig::initMap();

// ---------------- assign() ----------------

void ServerConfig::assign(const std::string &key, const std::string &value)
{
    std::map<std::string, Setter>::const_iterator it = _setters.find(key);
    if (it != _setters.end())
        (this->*(it->second))(value);
    else
        error_message("Unknown config key: " + key);
}