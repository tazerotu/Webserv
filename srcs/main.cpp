/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2025/11/18 10:07:33 by ttas             ###   ########.fr       */
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

	

	// Print ServerConfig contents via its getters.
	// Adjust getter names below to match your ServerConfig API if necessary.
	// try {
	// 	std::cout << "ServerConfig:" << std::endl;

	// 	// basic scalar values
	// 	std::cout << "  host: " << serverConfig.getHost() << std::endl;
	// 	std::cout << "  port: " << serverConfig.getPort() << std::endl;
	// 	std::cout << "  root: " << serverConfig.getRoot() << std::endl;
	// 	std::cout << "  max_body_size: " << serverConfig.getMaxBodySize() << std::endl;

	// 	// server names (vector<string>)
	// 	std::cout << "  server_names: ";
	// 	for (auto const& name : serverConfig.getServerNames())
	// 		std::cout << name << " ";
	// 	std::cout << std::endl;

	// 	// index files (vector<string>)
	// 	std::cout << "  index: ";
	// 	for (auto const& idx : serverConfig.getIndexFiles())
	// 		std::cout << idx << " ";
	// 	std::cout << std::endl;

	// 	// error pages (map<int,string> or similar)
	// 	auto errorPages = serverConfig.getErrorPages();
	// 	for (auto const& p : errorPages)
	// 		std::cout << "  error_page " << p.first << " -> " << p.second << std::endl;

	// 	// listeners / binds (vector<int> or vector<string>)
	// 	std::cout << "  listen: ";
	// 	for (auto const& l : serverConfig.getListen())
	// 		std::cout << l << " ";
	// 	std::cout << std::endl;

	// 	// locations (vector<Location>), print each location's getters
	// 	auto locations = serverConfig.getLocations();
	// 	for (size_t i = 0; i < locations.size(); ++i) {
	// 		auto const& loc = locations[i];
	// 		std::cout << "  location[" << i << "] path: " << loc.getPath() << std::endl;
	// 		std::cout << "    root: " << loc.getRoot() << std::endl;
	// 		std::cout << "    autoindex: " << (loc.getAutoIndex() ? "on" : "off") << std::endl;
	// 		std::cout << "    methods: ";
	// 		for (auto const& m : loc.getAllowedMethods())
	// 			std::cout << m << " ";
	// 		std::cout << std::endl;
	// 		std::cout << "    index: ";
	// 		for (auto const& idx : loc.getIndexFiles())
	// 			std::cout << idx << " ";
	// 		std::cout << std::endl;
	// 		auto cgih = loc.getCgiHandlers();
	// 		for (auto const& c : cgih)
	// 			std::cout << "    cgi: " << c.first << " -> " << c.second << std::endl;
	// 	}
	// } catch (std::exception const& e) {
	// 	std::cerr << "Error printing ServerConfig: " << e.what() << std::endl;
	// }
	return (0);
}