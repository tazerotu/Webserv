//
// Created by yannou on 02/02/2026.
//

#ifndef WEBSERV_REDIRECTRESPONSE_HPP
#define WEBSERV_REDIRECTRESPONSE_HPP
#include <string>

#include "HttpStatus.hpp"
#include "ParsingRequest.hpp"
#include "Response.hpp"

namespace webserv {
    namespace http {
        class RedirectResponse {
        public:
            static std::string buildRedirectResponse(
            const int code) {
                std::stringstream  body;
                body << "<html>\r\n"
                << "<head><title>" << code
                << " " << http::HttpStatus::getReasonPhrase(code)
                <<"</title></head>\r\n"
                << "<body>\r\n"
                << "<center><h1>" << code
                << " " << http::HttpStatus::getReasonPhrase(code)
                << "</body>\r\n"
                << "</html>\r\n";
                return body.str();
            }

            static Response processRedirectResponse(
                    const std::map<int, std::string> &redirMap,
                    const http::ParsingRequest& req,
                    const std::string& path) {
                std::map<int, std::string>::const_iterator it
                                                    = redirMap.begin();
                const int code = it->first;
                const std::string url = it->second;
                const std::string body = buildRedirectResponse(code);
                Response res;
                res.setStatusCode(code);
                res.addHeader("Location", url);
                res.addHeader("Content-Type", MimeTypes::getType(path));
                res.addHeader("Connection", req.getHeaderInfo("Connection"));
                res.setBody(body);
                return res;
            }
        };
    }
}
#endif //WEBSERV_REDIRECTRESPONSE_HPP