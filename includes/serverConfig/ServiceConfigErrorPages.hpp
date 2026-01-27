/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigErrorPages.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:32:30 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 16:09:08 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGERRORPAGES_HPP
# define SERVICECONFIGERRORPAGES_HPP

#include <string>
#include <cstring>
#include <cerrno>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <unistd.h>
#include "../http/HttpStatus.hpp"
#include "../serverConfig/IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        class ServiceConfigErrorPages {
        public:
            typedef std::map<int, std::string> Map;
        private:
            Map m_map;

        public:
            explicit ServiceConfigErrorPages(Map& map) : m_map(map) {}
            ~ServiceConfigErrorPages(){}

            static bool isValidErrorPage(Map &map) {
                for (Map::const_iterator it = map.begin(); it != map.end(); ++it) {
                    if (!http::HttpStatus::IsValidStatusCode(it->first))
                        return false;
                    //check for ErrorPage address
                }
                return true;
            }

            static ServiceConfigErrorPages create(Map map) {
                for (Map::const_iterator it = map.begin(); it != map.end();
                        ++it) {
                    if (!http::HttpStatus::IsValidStatusCode(it->first))
                        throw IServerConfigError::create(
                            invalid_error_page,NULL);
                    //std::string str = "../" + it->second;
                    std::string str = it->second;
                    int fd = 0;
                    std::cout << "file opening2:" << str << std::endl;
                    fd = open(str.c_str(), O_RDONLY);
                    if (fd == -1)
                        throw IServerConfigError::create(
                            invalid_error_directory,std::strerror(errno));
                    close(fd);
                }
                return ServiceConfigErrorPages(map);
            }

            Map& getValue() { return m_map; }

            const Map& getValue()const { return m_map; }
        };
    }
}
#endif
