//
// Created by yannou on 02/02/2026.
//

#ifndef WEBSERV_ERRORPAGEGENERATOR_HPP
#define WEBSERV_ERRORPAGEGENERATOR_HPP


#include <map>
#include <fstream>
#include <sstream>
#include "Response.hpp"
#include "HttpError.hpp"

namespace webserv {
    namespace http {

        class ErrorPageGenerator {
            public:
            static int mapErrnoToCode(int sysErrno) {
                
                switch (sysErrno) {
                    case ENOENT: return 404;
                    case EACCES:
                    case EPERM:
                    case EISDIR: return 403;
                    default: return 500;
                }
            }
            // 1. Try to find the custom error page
            // 2. Fallback: Generate a default simple HTML body
            static Response generate(int statusCode,
                    const std::map<int, std::string>& errorPages) {
                Response res;
                res.setStatusCode(statusCode);
                res.addHeader("Content-Type", "text/html");

                std::map<int, std::string>::const_iterator it =
                    errorPages.find(statusCode);
                if (it != errorPages.end()) {
                    std::ifstream file(it->second.c_str());
                    if (file.is_open()) {
                        std::stringstream buffer;
                        buffer << file.rdbuf();
                        if (buffer.str().size() > 0) {
                            res.setBody(buffer.str());
                            return res;
                        }
                    }
                }
                std::stringstream defaultBody;
                defaultBody << "<html><body><h1>" << statusCode << " "
                            << HttpStatus::getReasonPhrase(statusCode)
                            << "</h1></body></html>";
                res.setBody(defaultBody.str());
                return res;
            }
        };
    }
}

#endif //WEBSERV_ERRORPAGEGENERATOR_HPP