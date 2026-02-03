//
// Created by yannou on 30/01/2026.
//

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

namespace webserv {
    class RequestHandler {

    private:

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
            std::cout << "***Router::selectRoute(*config, uri): "
                << targetRoute->getRouteLoc().getValue() << std::endl;
            const std::string path = buildPath(req, targetRoute);
            int code = RequestValidator::validate(path, req, conf,
                                                   targetRoute);
            if (code != 200)
                return http::ErrorPageGenerator::generate(code,
                 conf.getErrorPages().getValue());
            http::Response res;
            const std::map<int, std::string> &redirMap
                                    = targetRoute->getRedirection().getValue();
            if (isRedirect(redirMap))
                return http::RedirectResponse::processRedirectResponse(
                    redirMap, req, path);
            if (isCGI(targetRoute))
                    return http::CgiHandler::processCGI(req, conf, targetRoute,
                        path);
            if (isFileToUpload(targetRoute))
                return http::FileToUpload::processFileToUpload(req, path,
                    conf.getErrorPages(),
                    conf.getMaxBodySize().getValue());
            // ... handle static files ...
            return http::StaticFileResponse::processStaticFile(path, conf.getErrorPages(),
                req.getHeaderInfo("Server"),
                req.getMethod(), req.getHeaderInfo("Connection"));;
        }

        static 	std::string buildPath(const http::ParsingRequest& req,
                    const serverConfig::routes::IServerConfigRoutes* targetRoute){
            //replace route location by root path
            std::cout << "***req.getPath()= " << req.getPath() << std::endl;
            std::cout << "***targetRoute.getRouteLoc().getValue()= "
                        << targetRoute->getRouteLoc().getValue() << std::endl;
            size_t pos =
                req.getPath().find_first_of(
                    targetRoute->getRouteLoc().getValue(), 0);
            if (pos ==  std::string::npos) {
                std::cerr << "No route location found!" << std::endl;
                pos = 0;
            }
            std::cout << "***pos= " << pos << std::endl;
            std::string uri = req.getPath().substr(pos
                + targetRoute->getRouteLoc().getValue().size());
            std::string path = "." + targetRoute->getRootPath().getValue()
                + uri;
            std::cout << "***file opening1:" << path << std::endl;
            return path;
        }
    };
}
#endif //WEBSERV_REQUESTHANDLER_HPP