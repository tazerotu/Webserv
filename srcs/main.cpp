/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2026/01/21 13:47:24 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

// Function to print a ServerConfigRoutes object
static void printRoute(const ServerConfigRoutes &route) {
    std::cout << "  Route Location: " << route.getRouteLoc() << "\n";
    std::cout << "  Root Path: " << route.getRootPath() << "\n";
    std::cout << "  Default File: " << route.getDefaultFile() << "\n";
    std::cout << "  Auto Index: " << static_cast<int>(route.getAutoIndex()) << "\n";
    std::cout << "  Upload Directory: " << route.getUploadDirectory() << "\n";
    std::cout << "  Upload: " << static_cast<int>(route.getUpload()) << "\n";
    std::cout << "  CGI Path: " << route.getCgiPath() << "\n";
    std::cout << "  CGI Ext: " << route.getCgiExt() << "\n";

    std::cout << "  Methods: ";
    for (size_t i = 0; i < route.getConfigMethods().size(); ++i) {
        std::cout << route.getConfigMethods()[i];
        if (i != route.getConfigMethods().size() - 1) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "  Redirections:\n";
    for (std::map<int, std::string>::const_iterator it = route.getRedirection().begin(); it != route.getRedirection().end(); ++it) {
        std::cout << "    " << it->first << " -> " << it->second << "\n";
    }
}

// Function to print a ServerConfig object
static void printServerConfig(const ServerConfig &config) {
    std::cout << "Server Name: " << config.getName() << "\n";
    std::cout << "Host: " << config.getHost() << "\n";
    std::cout << "Port: " << config.getPort() << "\n";
    std::cout << "Root: " << config.getRoot() << "\n";
    std::cout << "Max Client Body Size: " << config.getMaxClientBodySize() << "\n";

    std::cout << "Error Pages:\n";
    for (std::map<std::string, std::string>::const_iterator it = config.getErrorPages().begin(); it != config.getErrorPages().end(); ++it) {
        std::cout << "  " << it->first << " -> " << it->second << "\n";
    }

    std::cout << "Routes:\n";
    for (size_t i = 0; i < config.getRoutes().size(); ++i) {
        std::cout << "Route " << i + 1 << ":\n";
        printRoute(config.getRoutes()[i]);
    }
}


static void parse_config(std::vector<ServerConfig> *Configs, std::string config_file)
{
    if (config_file.size() < 5 || config_file.substr(config_file.size() - 5, 5) != ".conf")
    {
        perror("Invalid config file extension. Expected .conf");
        exit(1);
    }

    std::ifstream file(config_file.c_str());
    if (!file) {
        std::cerr << "Cannot open file\n";
        exit(1);
    }

    std::stringstream block;
    std::string line;
    bool inside = false;
    int braceCount = 0;

    while (std::getline(file, line)) {

        // Remove comments (everything after '#')
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }

        if (line.empty()) continue; // skip empty lines
        // Detect start of block
        if (!inside) {
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
                (*Configs).push_back(ServerConfig(block));
                continue; // don't include the closing brace
            }

            // Save content inside server block
            block << line << "\n";
        }
    }
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cout << "Wrong amount of arguments!" << std::endl << "correct syntax : ./webserv [path/to/file.conf]" << std::endl;
		return(0);	
	}

	std::vector<ServerConfig> Configs;
	Configs.clear();
	std::string file = argv[1];
	parse_config(&Configs, file);

	if (Configs.empty())
        return(0);
    else
    {
        for (size_t i = 0; i < Configs.size(); ++i) {
        std::cout << "========================\n";
        std::cout << "Server Configuration " << i + 1 << ":\n";
        printServerConfig(Configs[i]);
        std::cout << "========================\n\n";
    }
    }

	return (0);
}

