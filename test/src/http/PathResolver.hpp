/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathResolver.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 13:10:06 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 13:52:23 by yroard           ###   ########.fr       */
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
                std::string uri = req.getPath();
                std::string location = targetRoute->getRouteLoc().getValue();
                std::string root = targetRoute->getRootPath().getValue();          
                std::string suffix = "";
                if (uri.length() > location.length()) {
                    suffix = uri.substr(location.length()); // Donne "/test.txt"
                }
                if (!suffix.empty() && !root.empty() && root[root.size() - 1] == '/' && suffix[0] == '/') {
                    suffix = suffix.substr(1); // On enlève le slash du suffixe
                }
                // Si le root ne finit pas par '/' et le suffix ne commence pas par '/', on en ajoute un.
                std::string finalPath;
                if (!root.empty() && root[0] == '/') {
                    finalPath = "." + root + suffix;
                } else {
                    finalPath = root + suffix;
                }
                Logger::messagesFilter(DEBUG, "root: ", root);
                Logger::messagesFilter(DEBUG, "uriSuffix: ", suffix);    
                Logger::messagesFilter(DEBUG, "buildPath: ", finalPath);
                return finalPath;
			}
		};
	}
}

#endif
