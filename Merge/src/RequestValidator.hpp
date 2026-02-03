/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:34:28 by yroard            #+#    #+#             */
/*   Updated: 2026/01/28 16:02:27 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTVALIDATOR_HPP
# define REQUESTVALIDATOR_HPP

#include <sys/stat.h>
#include "http/ParsingRequest.hpp"
#include "serverConfig/ServerConfig.hpp"
#include "serverConfig/routes/IServerConfigRoutes.hpp"

namespace webserv {

    class RequestValidator {
    public:
        static int validate(std::string path,
                const webserv::http::ParsingRequest &request,
                const serverConfig::ServerConfig &config,
                const serverConfig::routes::IServerConfigRoutes *route){
            if (request.getBody().size() > config.getMaxBodySize().getValue()) {
                return 413; // Payload Too Large
            }
            const std::string& method = request.getMethod();
            struct stat buf = {};
            if (stat(path.c_str(), &buf) == -1
                    && route->getStoreStatus().getValue() == OFF)
                return 404; // Method Forbidden
            if (!route->getMethod().isAllowed(method)) {
                return 403; // Method Forbidden
            }
            if (request.getHttpVersion() != "HTTP/1.0"
                    && request.getHttpVersion() != "HTTP/1.1")
                return 505; // Version Not Supported
            return 200; // All checks passed
        }
    };
}
#endif


