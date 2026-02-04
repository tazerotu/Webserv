/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathResolver.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 13:10:06 by yroard            #+#    #+#             */
/*   Updated: 2026/02/04 09:32:52 by ttas             ###   ########.fr       */
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
                // FIX: Use find(), not find_first_of()
                size_t pos = requestUri.find(routeLocation);

                // 3. Error Handling via Exception
                if (pos == std::string::npos) {
                    // This is technically a 500 Internal Error because the Router 
                    // shouldn't have selected this route if it doesn't match.
                    // But 404 is safe too.
                    throw http::HttpError(500, 
						"PathResolver: URI does not match Route Location");
                }

                // 4. Construct the path
                // Remove the location part from the URI
                std::string uriSuffix = requestUri.substr(pos + routeLocation.size());
                
                // Construct: "." + root + suffix
                // Ensure root ends with '/' or uriSuffix starts with '/' to avoid "rooturi"
                std::string finalPath = "." + root + uriSuffix;

                std::cout << "*** Path Resolved: " << finalPath << std::endl;
                
                return finalPath;
			}
		};
	}
}

#endif
			// static const std::string buildPath(const http::ParsingRequest& req,
			// 	const serverConfig::routes::IServerConfigRoutes* targetRoute){				
			// 	//replace route location by root path
			// 	std::cout << "***req.getPath()= " << req.getPath() << std::endl;
			// 	std::cout << "***targetRoute.getRouteLoc().getValue()= "
			// 				<< targetRoute->getRouteLoc().getValue() << std::endl;
			// 	size_t pos =
			// 		req.getPath().find_first_of(
			// 			targetRoute->getRouteLoc().getValue(), 0);
			// 	if (pos ==  std::string::npos) {
			// 		//std::cerr << "No route location found!" << std::endl;
			// 		return http::ErrorPageGenerator::generateMsg(404,
            //         	conf.getErrorPages().getValue());
			// 	}
			// 	std::cout << "***pos= " << pos << std::endl;
			// 	std::string uri = req.getPath().substr(pos
			// 		+ targetRoute->getRouteLoc().getValue().size());
			// 	std::string path = "." + targetRoute->getRootPath().getValue()
			// 		+ uri;
			// 	std::cout << "***file opening1:" << path << std::endl;
			// 	return path;
			// }