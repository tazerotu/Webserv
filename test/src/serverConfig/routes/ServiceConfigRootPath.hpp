/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigRootPath.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:31:25 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 13:55:48 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGROOTPATH_HPP
# define SERVICECONFIGROOTPATH_HPP

#include <string>
#include <cstring>
#include <dirent.h>
#include <cerrno>

#include "../../Logger.hpp"
#include "../IServerConfigError.hpp"

namespace webserv {
	namespace serverConfig {
		namespace routes{
			class ServiceConfigRootPath {
			private:
				const std::string m_rootPath;
			public:
				explicit ServiceConfigRootPath(const std::string& rootPath)
					: m_rootPath(rootPath) {}
				~ServiceConfigRootPath(){}
				static ServiceConfigRootPath create(const std::string& rootPath){
					std::string path = "." + rootPath;
					Logger::messagesFilter(DEBUG, "path= ", path);
					if (rootPath.empty())
						throw IServerConfigError::create(invalid_root_path, std::strerror(errno));
					DIR* dir = opendir(path.c_str());
					if (!dir)
						throw IServerConfigError::create(invalid_root_path, std::strerror(errno));
					closedir(dir);
					return ServiceConfigRootPath(rootPath);
				}
				const std::string& getValue()const { return m_rootPath; }
			};
		}
	}
}
#endif
