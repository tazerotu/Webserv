/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigCGI.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:30:32 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 09:52:54 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVICECONFIGCGI_HPP
# define SERVICECONFIGCGI_HPP

#include <string>
#include <unistd.h>
#include <vector>

#include "../IServerConfigError.hpp"

namespace webserv {
	namespace serverConfig {
		namespace routes {
			typedef std::map<std::string, std::vector<std::string> > Map;
			class ServiceConfigCGI {
			private:
				const std::string m_cgiInterpreterPath;
				const std::string m_cgiExtension;

			public:
				explicit ServiceConfigCGI(const std::string& cgiInterpreterPath,
					const std::string& cgiExtension)
						: m_cgiInterpreterPath(cgiInterpreterPath),
						m_cgiExtension(cgiExtension){}

				~ServiceConfigCGI(){}

				static bool isExecutable(const std::string &path) {
					if (path.empty())
						return false;
					if (access(path.c_str(), X_OK ) == 0){
							return true;
					}
					return false;
				}

				static ServiceConfigCGI create(
						const std::string & cgiInterpreterPath,
						const std::string &cgiExtension) {
					if (cgiExtension.empty() && cgiInterpreterPath.empty()) {
						return ServiceConfigCGI("", "");
					}
					if (cgiExtension.empty() || cgiInterpreterPath.empty()) {
						throw IServerConfigError::create(invalid_CGI, NULL);
					}
					if (!isExecutable(cgiInterpreterPath)) {
						throw IServerConfigError::create(
							Invalid_CGI_interpreter, NULL);
					}
					return ServiceConfigCGI(cgiInterpreterPath, cgiExtension);
				}
				
				// Getters
				const std::string &getCGIInterpreterPath() const { 
					return m_cgiInterpreterPath; 
				}
				
				const std::string &getCGIExtension() const { 
					return m_cgiExtension;
				}
				
				bool hasCGI() const {
					return !m_cgiInterpreterPath.empty() && !m_cgiExtension.empty();
				}
			};
		}
	}
}
#endif
