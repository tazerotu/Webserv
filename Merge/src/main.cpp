/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:34:18 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 10:39:10 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <csignal>
#include <climits>
#include <set>

#include "serverConfig/ServerConfig.hpp"
#include "RequestValidator.hpp"
#include "Router.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "test/test.hpp"
#include "serverConfig/IServerConfig.hpp"
#include "serverConfig/routes/SCMethodFactory.hpp"
#include "http/ParsingRequest.hpp"
#include "http/Response.hpp"
#include "ServerManager.hpp"
#include "parsing/Parsing.hpp"

using namespace webserv::serverConfig;
using namespace webserv::serverConfig::routes;

namespace webserv {
    bool Init::stopRequested = false; 
}

void signalHandler(int signum) {
    (void)signum;
    webserv::Init::stopRequested = true;
}

bool arePortsDifferent(std::vector <ServerConfig*> tabServerConf) {
	size_t index2 = 0;
	for (size_t index1 = 0; index1 < tabServerConf.size(); ++index1) {
		for (index2 = index1 + 1; index2 < tabServerConf.size(); ++index2){
			if (tabServerConf[index1]->getPort().getValue()
					== tabServerConf[index2]->getPort().getValue())
				return false;
		}
	}
	return true;
}

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
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }

        if (line.empty()) continue;
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
            if (line.find("{") != std::string::npos)
                braceCount++;

            if (line.find("}") != std::string::npos)
                braceCount--;

            if (braceCount == 0) {
                inside = false;
                (*Configs).push_back(Parsing(block));
                continue;
            }

            block << line << "\n";
        }
    }
}

/* hard coded server config
{
webserv::test::testIPAddress();
	webserv::test::testPort();
	webserv::test::testMaxBodySize();
	std::map<int, std::string> noRedirMap;
	std::map<int, std::string> redirMap;
	std::string absPath = "./";
	//"/home/yannou/Bureau/Ecole_42/PROJETS/COMMON_CORE/commonCore05/core05-webserv/webserv";
	redirMap[301] = "http://dermofluide.com"; // Example
	std::map<int, std::string> errorPagesMap;
	errorPagesMap[400] = "./pages/Alps/error/400.html";
	errorPagesMap[401] = "./pages/Alps/error/401.html";
	errorPagesMap[403] = "./pages/Alps/error/403.html";
	errorPagesMap[404] = "./pages/Alps/error/404.html";
	errorPagesMap[500] = "./pages/Alps/error/500.html";
	errorPagesMap[502] = "./pages/Alps/error/502.html";
	errorPagesMap[503] = "./pages/Alps/error/503.html";
	errorPagesMap[504] = "./pages/Alps/error/504.html";
	//Check for redirection
	std::cout << "route 1 starting to be  defined" << std::endl;
	IServerConfigRoutes* route1 = new ServerConfigRoutes(
			ServiceConfigRouteLoc::create("/old-page"),
			SCMethodFactory::createMethod("GET POST DELETE"),
			ServiceConfigRedirection::create(redirMap),
			ServiceConfigRootPath::create(""),
			ServiceConfigAutoIndex::create(0),
			ServiceConfigDefaultFile::create(""),
			ServiceConfigStoreStatus::create("OFF"),
			ServiceConfigCGI::create("", ""));
	std::cout << "route 1 defined" << std::endl;	
	TabRoute routesVec1;
	routesVec1.push_back(route1);


	std::cout << "route 2 starting to be  defined" << std::endl;
	//Test with http://localhost:9000/index.html
	IServerConfigRoutes* route2 = new ServerConfigRoutes(
	ServiceConfigRouteLoc::create(("/")),
	SCMethodFactory::createMethod("GET"),
	ServiceConfigRedirection::create(noRedirMap),
	ServiceConfigRootPath::create("/pages/Alps/"),
	ServiceConfigAutoIndex::create(0),
	ServiceConfigDefaultFile::create("index.html"),
	ServiceConfigStoreStatus::create("OFF"),
	ServiceConfigCGI::create("", ""));
	routesVec1.push_back(route2);
	ServerConfig* serverConf1 = new ServerConfig(
		ServiceConfigWebsiteName::create("Hello"),
		ServiceConfigIPAddress::create("127.0.0.1"),
		ServiceConfigPort::create(8080),
		ServiceConfigErrorPages::create(errorPagesMap),
		ServiceConfigMaxBodySize::create(1000),
		routesVec1);
	std::vector <ServerConfig*> tabServerConf;
	tabServerConf.push_back(serverConf1);
	std::cout << "route 3 starting to be  defined" << std::endl;
	IServerConfigRoutes* route3 = new ServerConfigRoutes(
	 	ServiceConfigRouteLoc::create(("/fileToUpload/")),
		SCMethodFactory::createMethod("POST"),
		ServiceConfigRedirection::create(noRedirMap),
		ServiceConfigRootPath::create("/pages/Alps/uploads/"),
		ServiceConfigAutoIndex::create(0),
		ServiceConfigDefaultFile::create(""),
		ServiceConfigStoreStatus::create("ON"),
		ServiceConfigCGI::create("", ""));
	std::cout << "route 4 starting to be  defined" << std::endl;
	IServerConfigRoutes* route4 = new ServerConfigRoutes(
	 	ServiceConfigRouteLoc::create(("/uploads/")),
		SCMethodFactory::createMethod("GET"),
		ServiceConfigRedirection::create(noRedirMap),
		ServiceConfigRootPath::create("/pages/Alps/uploads/"),
		ServiceConfigAutoIndex::create(0),
		ServiceConfigDefaultFile::create(""),
		ServiceConfigStoreStatus::create("OFF"),
		ServiceConfigCGI::create("", ""));
	//http://localhost:9000/cgi/test.php
	std::cout << "route 5 starting to be  defined" << std::endl;
	IServerConfigRoutes* route5 = new ServerConfigRoutes(
	ServiceConfigRouteLoc::create(("/cgi/")),
	SCMethodFactory::createMethod("GET POST"),
	ServiceConfigRedirection::create(noRedirMap),
	ServiceConfigRootPath::create("/cgi/"),
	ServiceConfigAutoIndex::create(0),
	ServiceConfigDefaultFile::create(""),
	ServiceConfigStoreStatus::create("OFF"),
	ServiceConfigCGI::create("/usr/bin/php-cgi", ".php"));
	TabRoute routesVec2;
	routesVec2.push_back(route2);
	routesVec2.push_back(route3);
	routesVec2.push_back(route4);
	routesVec2.push_back(route5);
	ServerConfig* serverConf2 = new ServerConfig(
		ServiceConfigWebsiteName::create("Hello"),
		ServiceConfigIPAddress::create("127.0.0.1"),
		ServiceConfigPort::create(9000),
		ServiceConfigErrorPages::create(errorPagesMap),
		ServiceConfigMaxBodySize::create(1200),
		routesVec2);
	tabServerConf.push_back(serverConf2);
	// virtual const Routes::ServiceConfigRootPath &rootPath() = 0;
	// virtual const Routes::ServiceConfigErrorPages &errorPages() = 0;
	// virtual Routes::ServiceConfigAutoIndex autoIndex() = 0;
	if (!arePortsDifferent(tabServerConf)) {
		std::cout << "virtual hosting is not handled into this webserv";
		std::cout << ": ports must be different";
		return 1;
	}
}*/

std::vector<ServerConfig *> init_server_config(std::vector<ServerConfig*> Server, char *conf)
{
	std::vector<Parsing> Configs;
	Configs.clear();
	Server.clear();
	std::string file = conf;
	parse_config(&Configs, file);
	try
	{
		if (Configs.empty())
		{
			std::cout << "conf file is empty or invalid" << std::endl;
			exit(0);
		}
		else
		{
			for (size_t i = 0; i < Configs.size(); ++i) 
			{
				std::cout << "========================\n";
				std::cout << "Server Configuration " << i + 1 << ":\n";
				// printParsing(Configs[i]);
				std::cout << "========================\n\n";
			
				TabRoute routesVec;

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
						Configs[i].getMaxClientBodySize()),
					routesVec ));
			}
			// printServers(Server);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		exit(0);
	}	
	return (Server);
}



int main(int argc, char**argv) {
	
	if(argc != 2)
	{
		std::cout << "Wrong amount of arguments!" << std::endl << "correct syntax : ./webserv [path/to/file.conf]" << std::endl;
		return(0);	
	}

	signal(SIGINT, signalHandler);
	std::vector<ServerConfig *> tabServerConf;
	tabServerConf = init_server_config(tabServerConf, argv[1]);

	if (!arePortsDifferent(tabServerConf)) {
		std::cout << "virtual hosting is not handled into this webserv";
		std::cout << ": ports must be different";
		return 1;
	}
	try{
		webserv::IIOMultiplexer* multiplexer = new webserv::SelectMultiplexer();
		webserv::ServerManager serverManager(multiplexer, tabServerConf);
		std::cout << "MAIN LOOP" << std::endl;
		while (!webserv::Init::stopRequested) {
        	serverManager.run();
    	}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
