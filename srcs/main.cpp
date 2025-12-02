/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2025/12/02 10:21:56 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

static void print_config_data(ServerConfig serverConfig)
{
	std::cout << "------------------Data in class---------------------\n" << std::endl; 

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
}


int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cout << "Wrong amount of arguments!" << std::endl << "correct syntax : ./webserv [path/to/file.conf]" << std::endl;
		return(0);	
	}

	std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

	std::vector<ServerConfig> Configs;
    std::stringstream block;
    std::string line;
    bool inside = false;
    int braceCount = 0;

    while (std::getline(file, line)) {
        // Detect start of block
        if (!inside) {
            // Allow variations: "server {", "server{", "server   {", etc.
            if (line.find("server") != std::string::npos &&
                line.find("{") != std::string::npos) {
                inside = true;
                braceCount = 1; // found the first '{'
				block.str("");  // clear previous content
                block.clear();  // reset flags
                continue;       // don't include this line
            }
        } 
        else {
            // Already inside block
            if (line.find("{") != std::string::npos)
                braceCount++;

            if (line.find("}") != std::string::npos)
                braceCount--;

            // Stop if block ended
            if (braceCount == 0) {
                inside = false;
				Configs.push_back(ServerConfig(block));
                continue; // don't include the closing brace
            }

            // Save content inside server block
            block << line << "\n";
        }
    }

    // Now you can use block like any stringstream
    std::cout << "--- Extracted server block ---\n";
    std::cout << block.str() << std::endl;

	if (Configs.empty())
        return(0);
    else
    {
        for (std::vector<ServerConfig>::const_iterator it = Configs.begin();
            it != Configs.end(); ++it)
        {
            print_config_data(*it);
            if (it + 1 != Configs.end())
				std::cout << std::endl;
        }
    }

	return (0);
}

