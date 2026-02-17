/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathResolver.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 13:10:06 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 09:50:44 by ttas             ###   ########.fr       */
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
                std::string requestUri = req.getPath();
                std::string routeLocation = targetRoute->getRouteLoc().getValue();
                std::string root = targetRoute->getRootPath().getValue();
                size_t pos = requestUri.find(routeLocation);
                if (pos == std::string::npos) {
                    throw http::HttpError(500,
						"PathResolver: URI does not match Route Location");
                }
                std::string uriSuffix = requestUri.substr(pos + routeLocation.size());
                std::string finalPath = "." + root + uriSuffix;
                Logger::MessagesFilter(DEBUG,
                	 "buildPath: ", finalPath);
                return finalPath;
			}
		};
	}
}

#endif
