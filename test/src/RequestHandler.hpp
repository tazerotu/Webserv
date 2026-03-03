/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 13:13:55 by yroard            #+#    #+#             */
/*   Updated: 2026/03/03 09:49:06 by yroard           ###   ########.fr       */
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
#include "http/AutoIndex.hpp"

namespace webserv {
    class RequestHandler {
    private:
        RequestHandler(){}
        
        static bool isRedirect(const std::map<int, std::string>& redirMap) {
            if (!redirMap.empty())
                return true;
            return false;
        }

        static bool isCGIFile(
                const serverConfig::routes::IServerConfigRoutes* targetRoute,
                const std::string& path){
            if (targetRoute->getCGI().hasCGI()
                && path.find(".php") != std::string::npos)
                return true;
            return false;
        }

        static bool isCGIDefaultFile(
                const serverConfig::routes::IServerConfigRoutes* targetRoute,
                const std::string& path) {
            struct stat buf = {};
            if (stat(path.c_str(), &buf) == -1)
                return false;
            if(S_ISDIR(buf.st_mode))
            {
                if (targetRoute->getDefaultFile().getValue().empty())
                    return false;
                if (targetRoute->getCGI().hasCGI()
                    && targetRoute->getDefaultFile().getValue().find(".php")
                        != std::string::npos)
                    return true;
            }
            return false;
        }

        static bool isFileToUploadFile(
                const serverConfig::routes::IServerConfigRoutes* targetRoute,
                std::string& path) {
            if (targetRoute->getStoreStatus().getValue() == ON
                    && path.find(".php") != std::string::npos)
                return true;
            return false;
        }

        static bool isFileToUploadDefaultFile(
                const serverConfig::routes::IServerConfigRoutes* targetRoute,
                const std::string& path) {
            struct stat buf = {};
            if (stat(path.c_str(), &buf) == -1)
                return false;
            if(S_ISDIR(buf.st_mode))
            {
                if (targetRoute->getDefaultFile().getValue().empty())
                    return false;
                if (targetRoute->getCGI().hasCGI()
                    && targetRoute->getDefaultFile().getValue().find(".php")
                        != std::string::npos)
                    return true;
            }
            return false;
        }

        static bool isDirectoryListingRequested(
                const serverConfig::routes::IServerConfigRoutes* targetRoute,
                std::string& path){
            struct stat buf = {};
            if (stat(path.c_str(), &buf) == -1)
                return false;
            if(S_ISDIR(buf.st_mode))
            {
                if (!targetRoute->getDefaultFile().getValue().empty())
                    return false;
                if (targetRoute->getAutoIndex().getValue())
                    return true;
            }
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
            if (isCGIFile(targetRoute, path) || isCGIDefaultFile(targetRoute, path))
			{
                if (isCGIDefaultFile(targetRoute, path))
                    path += "/" + targetRoute->getDefaultFile().getValue();
				http::Response res =
                    http::CgiHandler::processCGI(conf, req, path);
				if ((res.getStatusCode() >= 400 && res.getStatusCode() <= 451)
                        || (res.getStatusCode() >= 500 && res.getStatusCode() <= 511))
					return http::ErrorPageGenerator::generate(res.getStatusCode(),
                        conf.getErrorPages().getValue());
				return (res);
			}
            if (isFileToUploadFile(targetRoute, path) || isFileToUploadDefaultFile(targetRoute, path)) {
                if (isFileToUploadDefaultFile(targetRoute, path))
                    path += "/" + targetRoute->getDefaultFile().getValue();
                return http::FileToUpload::processFileToUpload(req, path,
                    conf.getErrorPages(),
                    conf.getMaxBodySize().getValue());
            }
            if (isDirectoryListingRequested(targetRoute, path))
            {
                return http::AutoIndex::processDirListingResponse(req, conf, path);
            }
            return http::StaticFileResponse::processStaticFile(path, conf.getErrorPages(),
                req.getHeaderInfo("Server"),
                req.getMethod(), req.getHeaderInfo("Connection"), targetRoute);
        }
    };
}
#endif //WEBSERV_REQUESTHANDLER_HPP