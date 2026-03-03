//
// Created by yannou on 02/02/2026.
//

#ifndef WEBSERV_STATICFILERESPONSE_HPP
#define WEBSERV_STATICFILERESPONSE_HPP
#include "../ConvUtils.hpp"
#include "HttpError.hpp"
#include "Response.hpp"


namespace webserv {
    namespace http {
        class StaticFileResponse {
        private:
            static void ReadFileContent(std::stringstream *buffer,
                    const int fd) {
                char buf[4096];
                ssize_t ret;
                while ((ret = read(fd, buf, 1024)) > 0) {
                    buffer->write(buf, ret);
                }
            	if (buffer->fail()) {
            		close(fd);
            		throw HttpError::create(HttpStatus(InternalServerError),
						writing_to_stringstream_failure);
            	}
                close(fd);
            	if (ret == -1) {
            		throw HttpError::create(HttpStatus(InternalServerError),
						read_failure);
            	}
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
						std::cerr << "stat" << std::strerror(errno) << std::endl;
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
            	try {
            		ReadFileContent(&buffer, fd);
            	}
            	catch(const std::exception& e){
            		Logger::messagesFilter(ERR,
            			"File Read Error: ", e.what());
            		return ErrorPageGenerator::generate(500,
            			errorPages.getValue());
            	}
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
					const std::string& connection, 
					const serverConfig::routes::IServerConfigRoutes* targetRoute) {
    			std::string fullPath = path;
				struct stat buf = {};
				int statReturn = stat(fullPath.c_str(), &buf);
				if (statReturn == -1 || (S_ISDIR(buf.st_mode) 
						&& targetRoute->getDefaultFile().getValue().empty()))
					return ErrorPageGenerator::generate(403,
							errorPages.getValue());
				if (S_ISDIR(buf.st_mode)) {
					if (targetRoute->getDefaultFile().getValue().find_first_of("/") == 0)
						fullPath += targetRoute->getDefaultFile().getValue();
					else
						fullPath += "/" + targetRoute->getDefaultFile().getValue();
				}
				Logger::messagesFilter(DEBUG,
					"processStaticFile, path= ",
					fullPath);
				int fd = 0;
				fd = open(fullPath.c_str(), O_RDONLY);
				if (fd == -1)
					return ErrorPageGenerator::generate(404,
						errorPages.getValue());
				return buildStaticFileResponse(fd, fullPath, errorPages,
					serverName, method, connection);
			}
        };
    }
}
#endif //WEBSERV_STATICFILERESPONSE_HPP