/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:31:12 by ttas              #+#    #+#             */
/*   Updated: 2025/11/17 18:30:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig(std::string conf)
{
	std::fstream file;
	std::string line;

	if((conf.size() < 5) || (conf.substr(conf.size() - 5) != ".conf"))
	{
		std::cout << "Invalid config file extension. Expected .conf" << std::endl;
		exit(0);
	}

	file.open(conf, std::ios::in);
	
	if(!file.is_open())
	{
		std::cout << "Empty of non existant config file" << std::endl;
		exit(0);
	}
	
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		if(line.empty() || line[0] == '#')
			continue;
		int pos = line.find_first_of(" ");
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);
		std::cout << "Key: " << key << " | Value: " << value << std::endl;
		
	}
	
	file.close();	
		
}

std::string const 	ServerConfig::&getName(void);
std::string const 	ServerConfig::&getHost(void);
int                	ServerConfig::getPort(void);
std::string const 	ServerConfig::&getRoot(void);
std::string const 	ServerConfig::&getIndex(void);
std::string const 	ServerConfig::&getErrorPage(void);
std::string const 	ServerConfig::*getAllowedMethods(void);
int                	ServerConfig::getMaxClientBodySize(void);
std::string const 	ServerConfig::&getCgiPath(void);
std::string const 	ServerConfig::&getCgiExt(void);
void 				ServerConfig::setName(std::string const &name);
void 				ServerConfig::setHost(std::string const &host);
void 				ServerConfig::setPort(int port);
void 				ServerConfig::setRoot(std::string const &root);
void 				ServerConfig::setIndex(std::string const &index);
void 				ServerConfig::setErrorPage(std::string const &error_page);
void 				ServerConfig::setAllowedMethods(std::string *allowed_methods);
void 				ServerConfig::setMaxClientBodySize(int size);
void 				ServerConfig::setCgiPath(std::string const &cgi_path);
void 				ServerConfig::setCgiExt(std::string const &cgi_ext);
