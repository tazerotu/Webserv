/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:34:52 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:34:55 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
# define ROUTER_HPP

#include "serverConfig/ServerConfig.hpp"

//create a new object (or not!) from a port and an IP address
namespace webserv {
    class Router {
    public:
        // Step 1: Handle Virtual Hosting (Host Header)
        // If you have multiple servers on port 8080 (e.g., example.com and test.com)
        static const serverConfig::ServerConfig* selectServer(
                const std::vector<serverConfig::ServerConfig*>& servers,
                const std::string &hostHeader) {
            for (size_t i = 0; i < servers.size(); ++i) {
                // Check if server_name matches Host header
                if (servers[i]->getWebsiteName().getValue() == hostHeader) {
                    return servers[i];
                }
            }
            // Default to the first server if no match found (Nginx behavior)
            return servers[0];
        }
        // Step 2: Handle Location Matching (Longest Prefix Match)
        static const serverConfig::routes::IServerConfigRoutes* selectRoute(
                const serverConfig::ServerConfig &server,
                const std::string &uri) {
            const std::vector<serverConfig::routes::IServerConfigRoutes*> &routes
                = server.getRoutes();
            const serverConfig::routes::IServerConfigRoutes* bestMatch = NULL;
            size_t bestMatchLength = 0;
            std::cout << "uri to find: " << uri << std::endl;
            std::cout << "routes.size(): " << routes.size() << std::endl;
            for (size_t i = 0; i < routes.size(); ++i) {
                std::string routePath = routes[i]->getRouteLoc().getValue();
                std::cout << "routePath found: " << routePath << std::endl;
                // Or getRoutePath()
                // Check if URI starts with this route path
                if (uri.find(routePath) == 0) {
                    // We want the LONGEST match (e.g. URI "/img/logo.png" matches "/" and "/img/")
                    // We pick "/img/"
                    if (routePath.length() > bestMatchLength) {
                        bestMatch = routes[i];
                        bestMatchLength = routePath.length();
                    }
                }
            }
            return bestMatch; // Can be NULL if no route matches (should have a default "/" route)
        }
    };
}

#endif
