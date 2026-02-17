/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 13:13:55 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 09:48:46 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUESTHANDLER_HPP
#define WEBSERV_REQUESTHANDLER_HPP

#include <cstdio>

#include "RequestValidator.hpp"
#include "Router.hpp"
#include "http/CgiHandler.hpp"
#include "http/FileToUpload.hpp"
#include "http/Response.hpp"
#include "http/ParsingRequest.hpp"
#include "serverConfig/ServerConfig.hpp"
#include "http/MimeTypes.hpp"
#include "http/HttpError.hpp"
#include "http/RedirectResponse.hpp"
#include "http/StaticFileResponse.hpp"
#include "http/PathResolver.hpp"

namespace webserv {
    class RequestHandler {
    private:
        RequestHandler(){}
        
        static bool isRedirect(const std::map<int, std::string>& redirMap) {
            if (!redirMap.empty())
                return true;
            return false;
        }

        static bool isCGI(
                const serverConfig::routes::IServerConfigRoutes* targetRoute){
            if (targetRoute->getCGI().hasCGI())
                return true;
            return false;
        }

        static bool isFileToUpload(
                const serverConfig::routes::IServerConfigRoutes* targetRoute) {
            if (targetRoute->getStoreStatus().getValue() == ON)
                return true;
            return false;
        }

    public:
        static http::Response handleRequest(const http::ParsingRequest& req,
                const serverConfig::ServerConfig& conf) {
            const std::string& uri = req.getPath();
            const serverConfig::routes::IServerConfigRoutes* targetRoute =
                Router::selectRoute(conf, uri);
            if (!targetRoute) {
                return http::ErrorPageGenerator::generate(404,
                    conf.getErrorPages().getValue());
            }
             std::string path;
            try {
                path = http::PathResolver::buildPath(req,
                    targetRoute);
            }
            catch (const http::HttpError& e) { 
                return http::ErrorPageGenerator::generate(e.getCode(),
                    conf.getErrorPages().getValue());
            }    
            int code = RequestValidator::validate(path, req, conf,
                targetRoute);
            if (code != 200)
                return http::ErrorPageGenerator::generate(code,
                	conf.getErrorPages().getValue());
            const std::map<int, std::string> &redirMap
                = targetRoute->getRedirection().getValue();
            if (isRedirect(redirMap))
                return http::RedirectResponse::processRedirectResponse(
                    redirMap, req, path);
            if (isCGI(targetRoute) 
                && path.find(".php") != std::string::npos)
				{
					http::Response res = http::CgiHandler::processCGI(req, targetRoute, path);
					if (res.getStatusCode() == 500)
						http::ErrorPageGenerator::generate(500, conf.getErrorPages().getValue());
					else
					return (res);
				}
            if (isFileToUpload(targetRoute))
                return http::FileToUpload::processFileToUpload(req, path,
                    conf.getErrorPages(),
                    conf.getMaxBodySize().getValue());
            return http::StaticFileResponse::processStaticFile(path, conf.getErrorPages(),
                req.getHeaderInfo("Server"),
                req.getMethod(), req.getHeaderInfo("Connection"), targetRoute);;
        }
    };
}
#endif //WEBSERV_REQUESTHANDLER_HPP