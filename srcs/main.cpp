/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2025/11/18 11:27:28 by ttas             ###   ########.fr       */
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

    if (serverConfig.getErrorPage())
        std::cout << "Error page: " << *(serverConfig.getErrorPage()) << std::endl;
    else
        std::cout << "Error page: (none)" << std::endl;

    if (serverConfig.getAllowedMethods())
        std::cout << "Allowed methods: " << *(serverConfig.getAllowedMethods()) << std::endl;
    else
        std::cout << "Allowed methods: (none)" << std::endl;

    std::cout << "Max client body size: " << serverConfig.getMaxClientBodySize() << std::endl;
    std::cout << "CGI path: " << serverConfig.getCgiPath() << std::endl;
    std::cout << "CGI extension: " << serverConfig.getCgiExt() << std::endl;

	return (0);
}
