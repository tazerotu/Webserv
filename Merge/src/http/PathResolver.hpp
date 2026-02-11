/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathResolver.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 13:10:06 by yroard            #+#    #+#             */
/*   Updated: 2026/02/04 09:54:08 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATHRESOLVER_HPP
# define PATHRESOLVER_HPP

namespace webserv {
    namespace http {
		class PathResolver{
		private:
			PathResolver(){}
				
		public:
			static const std::string buildPath(const http::ParsingRequest& req,
				const serverConfig::routes::IServerConfigRoutes* targetRoute){				
				// 1. Get Values
                std::string requestUri = req.getPath();
                std::string routeLocation = targetRoute->getRouteLoc().getValue();
                std::string root = targetRoute->getRootPath().getValue();
                // 2. Find the route location inside the URI
                size_t pos = requestUri.find(routeLocation);
                // 3. Error Handling via Exception
                if (pos == std::string::npos) {
                    throw http::HttpError(500,
						"PathResolver: URI does not match Route Location");
                }
                // 4. Construct the path
                // Remove the location part from the URI
                std::string uriSuffix = requestUri.substr(pos + routeLocation.size());
                // Construct: "." + root + suffix
                // Ensure root ends with '/' or uriSuffix starts with '/' to avoid "rooturi"
                std::string finalPath = "." + root + uriSuffix;
                Logger::MessagesFilter(DEBUG,
                	 "buildPath: ", finalPath);
                return finalPath;
			}
		};
	}
}

#endif
