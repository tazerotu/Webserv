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
                    const ParsingRequest& req,
                    const std::string& path,
                    const serverConfig::ServiceConfigErrorPages& errorPages,
                    const unsigned maxBodySize) {
                Response res;
                if (req.getMethod() != "POST"){
                    Logger::MessagesFilter(DEBUG,
                        "req.getMethod() != \"POST\"","");
                    return ErrorPageGenerator::generate(403,
                        errorPages.getValue());
                }
                size_t pos = path.rfind('/');
                std::string fileToUpload = path.substr(pos + 1);
                const std::string bodyLength=
                    req.getHeaderInfo("Content-Length");
                Logger::MessagesFilter(DEBUG,
                    "processFileToUpload, body size: ",
                    bodyLength);
                const std::string body = req.getBody().getBufferStr();
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
                if (ConvUtils::cStringToLong(bodyLength) > maxBodySize)
                    return ErrorPageGenerator::generate(413,
                        errorPages.getValue());
                const int result = checkUploadFile(path, body,
                    ConvUtils::cStringToLong(bodyLength));
                if (result != 0){
                    return ErrorPageGenerator::generate(result,
                        errorPages.getValue());
                }
                res.addHeader("Content-Type", MimeTypes::getType(path));
                res.addHeader("Connection",
                    req.getHeaderInfo("Connection"));
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
                    Logger::MessagesFilter(ERR,
                         "Could not open file for writing! Error: ",
                        ConvUtils::intToStr(errorValue));
                    if (errorValue == EACCES){
                        Logger::MessagesFilter(DEBUG,
                            "errorValue == EACCES", "");
                        return 403;// You might want to throw an exception here to return a 500 or 403 error
                    }
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