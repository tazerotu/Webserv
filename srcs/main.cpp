/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2025/11/22 08:35:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

// #include <iostream>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cout << "Wrong amount of arguments!" << std::endl << "correct syntax : ./webserv [path/to/file.conf]" << std::endl;
		return(0);	
	}
	class ServerConfig serverConfig(argv[1]);

	std::cout << "ServerConfig values:" << std::endl;
    std::cout << "Name: " << serverConfig.getName() << std::endl;
    std::cout << "Host: " << serverConfig.getHost() << std::endl;
    std::cout << "Port: " << serverConfig.getPort() << std::endl;
    std::cout << "Root: " << serverConfig.getRoot() << std::endl;
    std::cout << "Index: " << serverConfig.getIndex() << std::endl;
    std::cout << "Autoindex: " << serverConfig.getAutoindex() << std::endl;

    // ---- Error pages ----
    const std::map<std::string, std::string> &errors = serverConfig.getErrorPages();
    std::cout << "Error pages: ";
    if (errors.empty())
        std::cout << "(none)";
    else
    {
        bool first = true;
        for (std::map<std::string, std::string>::const_iterator it = errors.begin();
            it != errors.end(); ++it)
        {
            if (!first)
                std::cout << ", ";
            std::cout << it->first << ": " << it->second;
            first = false;
        }
    }
    std::cout << std::endl;

    // ---- Allowed methods ----
    const std::vector<std::string> &methods = serverConfig.getAllowedMethods();
    std::cout << "Allowed methods: ";
    if (methods.empty())
        std::cout << "(none)";
    else
    {
        for (std::vector<std::string>::const_iterator it = methods.begin();
            it != methods.end(); ++it)
        {
            std::cout << *it;
            if (it + 1 != methods.end())
                std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Max client body size: " << serverConfig.getMaxClientBodySize() << std::endl;
    std::cout << "CGI path: " << serverConfig.getCgiPath() << std::endl;
    std::cout << "CGI extension: " << serverConfig.getCgiExt() << std::endl;

	return (0);
}
