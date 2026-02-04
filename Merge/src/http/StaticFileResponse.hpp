//
// Created by yannou on 02/02/2026.
//

#ifndef WEBSERV_STATICFILERESPONSE_HPP
#define WEBSERV_STATICFILERESPONSE_HPP
#include "Response.hpp"

namespace webserv {
    namespace http {
        class StaticFileResponse {
        private:
            static void ReadFileContent(std::stringstream *buffer,
                    const int fd) {
                // Read file content
                //std::stringstream buffer;
                char buf[1024];
                int ret;
                while ((ret = static_cast<int>(read(fd, buf, 1024))) > 0) {
                    buffer->write(buf, ret);
                }
                //TO DO what if ret <= 0? what if buffer == NULL?
                close(fd);
            }
            public:
            static Response buildStaticFileResponse(
                        const int fd, const std::string& path,
                        const serverConfig::ServiceConfigErrorPages& errorPages,
                        const std::string& serverName,
                        const std::string& method,
                        const std::string& connection) {
                if (method == "POST")
                    return ErrorPageGenerator::generate(405,
                        errorPages.getValue());
                struct stat buf = {};
                Response res;
                if (method == "DELETE") {
                    if (stat((path.c_str()), &buf) == -1) {
                        const int errorCode = errno;
                        perror("stat");
                        return ErrorPageGenerator::generate(
                            ErrorPageGenerator::mapErrnoToCode(errorCode),
                            errorPages.getValue());
                    }
                    if (buf.st_mode == S_IFDIR){
                        std::cout << "buf.st_mode == S_IFDIR" << std::endl;
                        return ErrorPageGenerator::generate(403,
                            errorPages.getValue());
                    }

                    if (std::remove(path.c_str()) != 0) {
                        const int errorCode = errno;
                        return ErrorPageGenerator::generate(
                            ErrorPageGenerator::mapErrnoToCode(errorCode),
                            errorPages.getValue());
                    }
                    res.setStatusCode(204);
                    res.addHeader("Server", serverName);
                    res.addHeader("Connection", connection);
                    return res;
                }
                std::stringstream buffer;
                ReadFileContent(&buffer, fd);
                std::string body = buffer.str();
                res.setStatusCode(200);
                res.addHeader("Content-Type",
                    MimeTypes::getType(path));
                res.addHeader("Connection", connection);
                res.setBody(body);
                return res;
            }

            static Response processStaticFile(const std::string& path,
            const serverConfig::ServiceConfigErrorPages& errorPages,
            const std::string& serverName,
            const std::string& method,
            const std::string& connection) {
                int fd = 0;
                fd = open(path.c_str(), O_RDONLY);
                if (fd == -1)
                    return ErrorPageGenerator::generate(404,
                        errorPages.getValue());
                return buildStaticFileResponse(fd, path, errorPages,
                    serverName, method, connection);
            }
        };
    }
}
#endif //WEBSERV_STATICFILERESPONSE_HPP