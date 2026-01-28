/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2026/01/28 09:38:24 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Parsing.hpp"
#include "../includes/serverConfig/IServerConfig.hpp"
#include "../includes/serverConfig/ServerConfig.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

using namespace webserv::serverConfig;
using namespace webserv::serverConfig::routes;

// Function to print a ParsingRoutes object
static void printRoute(const ParsingRoutes &route) {
    std::cout << "  Route Location: " << route.getRouteLoc() << "\n";
    std::cout << "  Root Path: " << route.getRootPath() << "\n";
    std::cout << "  Default File: " << route.getDefaultFile() << "\n";
    std::cout << "  Auto Index: " << static_cast<int>(route.getAutoIndex()) << "\n";
    std::cout << "  Upload Status: " << static_cast<int>(route.getUpload()) << "\n";
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

// Function to print a Parsing object
static void printParsing(Parsing &config) {
    std::cout << "Server Name: " << config.getName() << "\n";
    std::cout << "Host: " << config.getHost() << "\n";
    std::cout << "Port: " << config.getPort() << "\n";
    std::cout << "Root: " << config.getRoot() << "\n";
    std::cout << "Max Client Body Size: " << config.getMaxClientBodySize() << "\n";

    std::cout << "Error Pages:\n";
    for (std::map<int, std::string>::const_iterator it = config.getErrorPages().begin(); it != config.getErrorPages().end(); ++it) {
        std::cout << "  " << it->first << " -> " << it->second << "\n";
    }

    std::cout << "Routes:\n";
    for (size_t i = 0; i < config.getRoutes().size(); ++i) {
        std::cout << "Route " << i + 1 << ":\n";
        printRoute(config.getRoutes()[i]);
    }
}


void printServers(const std::vector<ServerConfig*>& servers)
{
    for (size_t i = 0; i < servers.size(); ++i)
    {
        const ServerConfig* s = servers[i];
        if (!s)
            continue;

        std::cout << "=================================\n";
        std::cout << "Server #" << i << "\n";

        // Server config values
        std::cout << "WebsiteName: " << s->getWebsiteName().getValue() << "\n";
        std::cout << "IP Address: " << s->getIPAddress().getValue() << "\n";
        std::cout << "Port: " << s->getPort().getValue() << "\n";
        std::cout << "MaxBodySize: " << s->getMaxBodySize().getValue() << "\n";

        // Error pages
        std::cout << "Error Pages:\n";
        const std::map<int, std::string>& errors = s->getErrorPages().getValue();
        for (std::map<int, std::string>::const_iterator it = errors.begin();
             it != errors.end(); ++it)
        {
            std::cout << "  " << it->first << " -> " << it->second << "\n";
        }

        // Routes
        const std::vector<IServerConfigRoutes*>& routes = s->getRoutes();
        std::cout << "Routes (" << routes.size() << "):\n";

        for (size_t j = 0; j < routes.size(); ++j)
        {
            const IServerConfigRoutes* r = routes[j];
            if (!r)
                continue;

            std::cout << "  Route #" << j << "\n";
            std::cout << "    Location: " << r->getRouteLoc().getValue() << "\n";
            std::cout << "    Method: " << r->getMethod().getValue() << "\n";

            const std::map<int, std::string>& redirMap =
                r->getRedirection().getValue();

            std::cout << "    Redirection:\n";
            for (std::map<int, std::string>::const_iterator it = redirMap.begin();
                 it != redirMap.end(); ++it)
            {
                std::cout << "      " << it->first << " -> " << it->second << "\n";
            }

            std::cout << "    Root: " << r->getRootPath().getValue() << "\n";
            std::cout << "    AutoIndex: " << r->getAutoIndex().getValue() << "\n";
            std::cout << "    DefaultFile: " << r->getDefaultFile().getValue() << "\n";
            std::cout << "    StoreStatus: " << r->getStoreStatus().getValue() << "\n";
            std::cout << "    CGI: "
                      << r->getCGI().getCGIInterpreterPath() << " "
                      << r->getCGI().getCGIExtension() << "\n";
        }
    }
}




static void parse_config(std::vector<Parsing> *Configs, std::string config_file)
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
                (*Configs).push_back(Parsing(block));
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

	std::vector<Parsing> Configs;
	std::vector<ServerConfig*> Server;
	Configs.clear();
	Server.clear();
	std::string file = argv[1];
	parse_config(&Configs, file);
	
	if (Configs.empty())
	return(0);
    else
    {
		for (size_t i = 0; i < Configs.size(); ++i) 
		{
			std::cout << "========================\n";
			std::cout << "Server Configuration " << i + 1 << ":\n";
			printParsing(Configs[i]);
			std::cout << "========================\n\n";
		
			TabRoute routesVec; // ✅ NEW vector for THIS server only

			for (size_t j = 0; j < Configs[i].getRoutes().size(); ++j)
			{
				std::string method_string;
				std::vector<std::string> methods =
					Configs[i].getRoutes()[j].getConfigMethods();

				for (size_t k = 0; k < methods.size(); ++k)
				{
					if (k > 0)
						method_string += " ";
					method_string += methods[k];
				}

				std::stringstream StoreStatus;
				StoreStatus << static_cast<int>(
					Configs[i].getRoutes()[j].getUpload()
				);

				routesVec.push_back(new ServerConfigRoutes(
					ServiceConfigRouteLoc::create(
						Configs[i].getRoutes()[j].getRouteLoc()
					),
					SCMethodFactory::createMethod(method_string),
					ServiceConfigRedirection::create(
						Configs[i].getRoutes()[j].getRedirection()
					),
					ServiceConfigRootPath::create(
						Configs[i].getRoutes()[j].getRootPath()
					),
					ServiceConfigAutoIndex::create(
						Configs[i].getRoutes()[j].getAutoIndex()
					),
					ServiceConfigDefaultFile::create(
						Configs[i].getRoutes()[j].getDefaultFile()
					),
					ServiceConfigStoreStatus::create(
						StoreStatus.str()
					),
					ServiceConfigCGI::create(
						Configs[i].getRoutes()[j].getCgiPath(),
						Configs[i].getRoutes()[j].getCgiExt()
					)
				));
			}

			Server.push_back(new ServerConfig(
				ServiceConfigWebsiteName::create(Configs[i].getName()),
				ServiceConfigIPAddress::create(Configs[i].getHost()),
				ServiceConfigPort::create(Configs[i].getPort()),
				ServiceConfigErrorPages::create(Configs[i].getErrorPages()),
				ServiceConfigMaxBodySize::create(
					Configs[i].getMaxClientBodySize()
				),
				routesVec
			));
    	}
		printServers(Server);
	}
	for (size_t i = 0; i < Server.size(); ++i)
    	delete Server[i];
	return (0);
}

