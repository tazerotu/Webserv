/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigDefaultFile.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:30:39 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:30:40 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SERVICECONFIGDEFAULTFILE_HPP
# define SERVICECONFIGDEFAULTFILE_HPP

#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include "../IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes {
            class ServiceConfigDefaultFile {
            private:
                const std::string m_defaultFile;

            public:
                explicit ServiceConfigDefaultFile(
                    const std::string &defaultFile)
                    : m_defaultFile(defaultFile) {}

                ~ServiceConfigDefaultFile(){}

                static ServiceConfigDefaultFile create(
                        std::string defaultFile) {
                    if (access(defaultFile.c_str(), F_OK ) != -1)
                        throw IServerConfigError::create(
                                invalid_default_file,
                                std::strerror(errno));
                    return ServiceConfigDefaultFile(defaultFile);
                }

                const std::string& getValue()const {
                    return m_defaultFile;
                }
            };
        }
    }
}
#endif
