/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 11:16:14 by yroard            #+#    #+#             */
/*   Updated: 2026/03/03 14:50:15 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef WEBSERV_AUTOINDEX_HPP
#define WEBSERV_AUTOINDEX_HPP

#include <cstdio>
#include <dirent.h>
#include <string>
#include <iostream>
#include <cerrno>
#include <cstring>

//When AutoIndex is set to 1 and that no default file has been set, then 
//response is sent with all the files in this directory
namespace webserv {
    namespace http {
        class AutoIndex {
        public:
            static std::string generateDirContent(const std::string& path) {
                DIR *resultOpenDir = opendir(path.c_str());
                if (!resultOpenDir){
					Logger::messagesFilter(ERR, "Error in opening directory",
                        std::strerror(errno));
                    return "";
                }
                std::string result;
                dirent* de;
                while (true){
                    de = readdir(resultOpenDir);
                    if (de == NULL)
                        break;
                    result += "<a href =";
                    result += '"';
                    result += std::string(de->d_name);
                    result += '"';
                    result += ">";
                    result += std::string(de->d_name);
                    result += "</a>";
                    result += "<br>";
                }
                closedir(resultOpenDir);
                return result;
            }

            static Response processDirListingResponse(
                    const http::ParsingRequest& req,
                    const serverConfig::ServerConfig& conf,
                    const std::string& path) {
                if (req.getMethod() == "POST" || req.getMethod() == "DELETE")
                    return ErrorPageGenerator::generate(405,
                        conf.getErrorPages().getValue());
                const std::string body = generateDirContent(path);
                Response res;
                res.setStatusCode(200);
                res.addHeader("Connection", req.getHeaderInfo("Connection"));
                res.setBody(body);
                return res;
            }
        };
    }
}
#endif //WEBSERV_AUTOINDEX_HPP