/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:34:52 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 13:51:17 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
# define ROUTER_HPP

#include "serverConfig/ServerConfig.hpp"

namespace webserv {
    class Router {
    public:
        static const serverConfig::ServerConfig* selectServer(
                const std::vector<serverConfig::ServerConfig*>& servers,
                const std::string &hostHeader) {
            for (size_t i = 0; i < servers.size(); ++i) {
                if (servers[i]->getWebsiteName().getValue() == hostHeader) {
                    return servers[i];
                }
            }
            return servers[0];
        }
        static const serverConfig::routes::IServerConfigRoutes* selectRoute(
                const serverConfig::ServerConfig &server,
                const std::string &uri) {
            const std::vector<serverConfig::routes::IServerConfigRoutes*> &routes
                = server.getRoutes();
            const serverConfig::routes::IServerConfigRoutes* bestMatch = NULL;
            size_t bestMatchLength = 0;
            Logger::messagesFilter(DEBUG,
                "uri to find: ", uri);
            for (size_t i = 0; i < routes.size(); ++i) {
                std::string routePath = routes[i]->getRouteLoc().getValue();
                Logger::messagesFilter(DEBUG,
                    "routePath found: ",
                    routePath);
                if (uri.find(routePath) == 0) {
                    if (routePath.length() > bestMatchLength) {
                        bestMatch = routes[i];
                        bestMatchLength = routePath.length();
                    }
                }
            }
            return bestMatch;
        }
    };
}

#endif
