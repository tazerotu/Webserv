/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:32:11 by yroard            #+#    #+#             */
/*   Updated: 2026/01/27 11:02:01 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "ServerConfig.hpp"
// #include "IServerConfig.hpp"
// #include "./routes/IServerConfigRoutes.hpp"

// #include <cstdlib> 

// ServerConfig* buildServerConfigFromParsing(const Parsing& parsing)
// {
// 	// Wrap primitive values into strong config types
// 	ServiceConfigWebsiteName websiteName(parsing.getName());
// 	ServiceConfigIPAddress   ipAddress(parsing.getHost());
// 	ServiceConfigPort        port(parsing.getPort());
// 	ServiceConfigErrorPages  errorPages(parsing.getErrorPages());
// 	ServiceConfigMaxBodySize maxBodySize(parsing.getMaxClientBodySize());
// 	// Build routes
// 	TabRoute routes;
// 	const std::vector<Parsing::ParsingRoutes>& parsingRoutes =
// 		parsing.getRoutes();
// 	for (size_t i = 0; i < parsingRoutes.size(); ++i)
// 	{
// 		routes.push_back(buildRouteFromParsing(parsingRoutes[i]));
// 	}
// 	return new ServerConfig(
// 		websiteName,
// 		ipAddress,
// 		port,
// 		errorPages,
// 		maxBodySize,
// 		routes
// 	);
// }