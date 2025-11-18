/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:31:12 by ttas              #+#    #+#             */
/*   Updated: 2025/11/18 10:14:36 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

#include <cstdlib> 

static void error_message(std::string message)
{
	std::cout << message << std::endl;
	exit(0);
}


bool isRegularFile(const std::string &path)
{
    struct stat st;

    // stat() returns 0 on success
    if (stat(path.c_str(), &st) != 0)
        return false;

    // S_ISREG tests for a regular file
    return S_ISREG(st.st_mode);
}

ServerConfig::ServerConfig(std::string conf)
{
	// void		(ServerConfig::*ptr_key[11])(void) = {};
	std::fstream file;
	std::string line;

	if((conf.size() < 5) || (conf.substr(conf.size() - 5) != ".conf"))
		error_message("Invalid config file extension. Expected .conf");

	file.open(conf.c_str(), std::ios::in);
	
	if(!file.is_open())
		error_message("Empty or non existant config file");
	
	if (!isRegularFile(conf))
        error_message("Config path must be a regular file, not a folder");
	

	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		if(line.empty() || line[0] == '#')
			continue;
		int pos = line.find_first_of(" ");
		if(!pos)
		{
			std::string msg = "Error : Invalid Config for line : " + line;
			error_message(msg);
		}

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);
		if(key.empty() || value.empty())
		{
			error_message("Error: Key or Value is empty");
		}
		
		std::cout << "Key: " << key << " | Value: " << value << "\n\n" << std::endl;
		
		
	}
	
	file.close();	
		
}

ServerConfig::~ServerConfig()
{}

// Getters
const std::string &ServerConfig::getName() const 		{return (this->_name);}
const std::string &ServerConfig::getHost() const 		{return (this->_host);}
int ServerConfig::getPort() const 						{return (this->_port);}
const std::string &ServerConfig::getRoot() const 		{return (this->_root);}
const std::string &ServerConfig::getIndex() const 		{return (this->_index);}
int ServerConfig::getAutoindex() const 					{return (this->_autoindex);}
std::string *ServerConfig::getErrorPage() const 		{return (this->_error_page);}
std::string *ServerConfig::getAllowedMethods() const 	{return (this->_allowed_methods);}
int ServerConfig::getMaxClientBodySize() const 			{return (this->_maxClientBodySize);}
const std::string &ServerConfig::getCgiPath() const 	{return (this->_cgi_path);}
const std::string &ServerConfig::getCgiExt() const 		{return (this->_cgi_ext);}


// Setters
void ServerConfig::setName(const std::string &name) 			{this->_name = name;}
void ServerConfig::setHost(const std::string &host) 			{this->_host = host;}
void ServerConfig::setPort(const std::string port) 				{this->_port = atoi(port.c_str());}
void ServerConfig::setRoot(const std::string &root)				{this->_root = root;}
void ServerConfig::setIndex(const std::string &index) 			{this->_index = index;}
void ServerConfig::setAutoindex(const std::string autoindex) 	{this->_autoindex = atoi(autoindex.c_str());}
void ServerConfig::setErrorPage(std::string *errorPage) 		{this->_error_page = errorPage;}
void ServerConfig::setAllowedMethods(std::string *methods) 		{this->_allowed_methods = methods;}
void ServerConfig::setMaxClientBodySize(const std::string size) {this->_maxClientBodySize = atoi(size.c_str());}
void ServerConfig::setCgiPath(const std::string &cgiPath)		{this->_cgi_path = cgiPath;}
void ServerConfig::setCgiExt(const std::string &cgiExt) 		{this->_cgi_ext = cgiExt;}