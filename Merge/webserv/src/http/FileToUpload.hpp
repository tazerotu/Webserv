//
// Created by yannou on 02/02/2026.
//

#ifndef WEBSERV_FILETOUPLOAD_HPP
#define WEBSERV_FILETOUPLOAD_HPP
#include "MimeTypes.hpp"
#include "ParsingRequest.hpp"
#include "../serverConfig/ServiceConfigErrorPages.hpp"
namespace webserv {
    namespace http {
        class FileToUpload {
        public:
            static Response processFileToUpload(
                    const webserv::http::ParsingRequest& req,
                    const std::string& path,
                    const webserv::serverConfig::ServiceConfigErrorPages& errorPages,
                    const unsigned maxBodySize) {
                Response res;
                if (req.getMethod() != "POST")
                    return ErrorPageGenerator::generate(403,
                        errorPages.getValue());
                size_t pos = path.rfind('/');
                std::string fileToUpload = path.substr(pos + 1);
                //std::cout << "***fileToUpload= [" << fileToUpload << "]" << std::endl;
                const std::string headerInfo =
                    req.getHeaderInfo("Content-Length");
                long bodySize = strtol(headerInfo.c_str(), NULL, 10);
                //TO DO
                //std::cout << "***bodySize= " << bodySize << std::endl;
                const std::string body = req.getBody().getBufferStr();
                // 1. Check if file exists BEFORE writing
                // access returns 0 if file exists, -1 if not
                const bool fileExists = (access(path.c_str(),
                    F_OK) == 0);
                int statusCode = 0;
                if (fileExists) {
                    statusCode = 200;
                    res.setStatusCode(statusCode);
                    res.setBody("File updated successfully");
                }
                else {
                    statusCode = 201;
                    res.setStatusCode(statusCode);
                    res.setBody("File created successfully");
                }
                if (bodySize > maxBodySize)
                    return ErrorPageGenerator::generate(413,
                        errorPages.getValue());
                const int result = checkUploadFile(path, body, bodySize);
                if (result != 0){
                    return ErrorPageGenerator::generate(result,
                        errorPages.getValue());
                }
                res.addHeader("Content-Type", MimeTypes::getType(path));
                res.addHeader("Connection",
                    req.getHeaderInfo("Connection"));
                std::stringstream ss;
                ss << body.size();
                res.addHeader("Content-Length", ss.str());
                if (statusCode == 201)
                    res.addHeader("Location", req.getPath());
                return res;
            }

        private:
            static int checkUploadFile(const std::string& path,
                    const std::string& body, long bodySize) {
                int errorValue = 0;
                int fd = open(path.c_str(),O_RDWR | O_CREAT | O_TRUNC, 00644);
                if (fd == -1) {
                    errorValue = errno;
                    std::cerr << "***Error: Could not open file for writing! Error: "
                        << errorValue << std::endl;
                    if (errorValue == EACCES)
                        return 403;// You might want to throw an exception here to return a 500 or 403 error
                    return 500;
                }
                const char* buffer = body.c_str();
                ssize_t resultWrite = write (fd, buffer, bodySize);
                close(fd);
                if (resultWrite == - 1) {
                    return 500;
                }
                return 0;
            }
        };
    }
}

#endif //WEBSERV_FILETOUPLOAD_HPP