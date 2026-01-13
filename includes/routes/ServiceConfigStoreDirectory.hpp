/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigStoreDirectory.hpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:31:43 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:31:46 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGSTOREDIRECTORY_HPP
# define SERVICECONFIGSTOREDIRECTORY_HPP

#include <string>
#include <dirent.h>
#include "../IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes {
            class ServiceConfigStoreDirectory {
            private:
                const std::string m_StoreDirectory;

            public:
                explicit ServiceConfigStoreDirectory(
                    const std::string &storeDirectory)
                    : m_StoreDirectory(storeDirectory) {}

                ~ServiceConfigStoreDirectory(){}

                static ServiceConfigStoreDirectory create(
                        const std::string & storeDirectory) {
                    if (!storeDirectory.empty() && !opendir(storeDirectory.c_str()))
                        throw IServerConfigError::create(
                                invalid_store_directory,NULL);
                    return ServiceConfigStoreDirectory(storeDirectory);
                }

                const std::string& getValue() {
                    return m_StoreDirectory;
                }
            };
        }
    }
}
#endif
