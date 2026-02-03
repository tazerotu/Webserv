//
// Created by yannou on 30/01/2026.
//

#ifndef WEBSERV_HTTPERROR_HPP
#define WEBSERV_HTTPERROR_HPP

#include <fcntl.h>
#include <unistd.h>
#include <exception>
#include <map>
#include <iostream>
#include <sstream>
#include "Response.hpp"
#include "HttpStatus.hpp"


namespace webserv {
    namespace http {
        typedef enum eExceptError {
            invalid_method,
            invalid_path,
            invalid_httpVersion,
            invalid_lineRequest,
            invalid_header,
            pipe_error,
            fork_error,
            execve_error,
            invalid_file
        }tExceptError;

        class HttpError : public std::exception {
        private:
            int m_code;
            std::string m_logMessage;
        public:
            explicit HttpError(const int code, const std::string& msg = "")
                : m_code(code), m_logMessage(msg) {}
            virtual ~HttpError() throw(){}

            static HttpError create(const HttpStatus status,
                                const tExceptError error) {
                std::string full_msg = "";
                full_msg += " : ";
                switch (error) {
                    case invalid_method:
                        full_msg = "method in this query is not handled";
                        break;
                    case invalid_path:
                        full_msg = "path is not handled properly";
                        break;
                    case invalid_httpVersion:
                        full_msg = "httpVersion is not handled properly";
                        break;
                    case invalid_lineRequest:
                        full_msg = "line request is not handled properly";
                        break;
                    case invalid_header:
                        full_msg = "header request is not handled properly";
                        break;
                    case pipe_error:
                        full_msg = "pipe error";
                        break;
                    case fork_error:
                        full_msg = "fork error";
                        break;
                    case execve_error:
                        full_msg = "execve error";
                        break;
                    case invalid_file:
                        full_msg = "error in opening file";
                        break;
                    default:
                        full_msg = "http error";
                }
                return HttpError(status.getStatusCodeInt(), full_msg);
            }

            int getCode() const throw() {
                return m_code;
            }

            virtual const char* what() throw() {
                return m_logMessage.empty() ? "HTTP Error" : m_logMessage.c_str();
            }

            static bool isErrorPageMissing(int* status,
                    const std::map<int, std::string>& errorPages,
                    int* fd){
                if (!http::HttpStatus::IsValidStatusCode(*status)) {
                    std::cerr << "status code is invalid: "
                        << *status << std::endl;
                    *status = 500;
                }
                std::map<int, std::string>::const_iterator it
                    = errorPages.find(*status);
                if (it != errorPages.end()) {
                    std::cout << "***it->second.c_str(): " << it->second.c_str() << std::endl;
                    *fd = open(it->second.c_str(), O_RDONLY);
                }
                if (*fd == -1)
                    return true;
                return false;
            }

            static bool isReadBufferOk(int fd,
                    std::stringstream* buffer) {
                char buf[1024];
                long retRead;
                while ((retRead =
                        static_cast<int>(read(fd, buf, 1024))) > 0) {
                    buffer->write(buf, retRead);
                }
                if (retRead == -1)
                    return false;
                return true;
            }

            // static Response sendError(const int statusCode,
            //         const std::map<int, std::string>& errorPages,
            //         const std::string& connection) {
            //     Response res;
            //     int status = statusCode;
            //     int fd = -1;
            //     res.setStatusCode(status);
            //     res.addHeader("Content-Type", "text/plain");
            //     res.addHeader("Connection", connection);
            //     std::stringstream buffer;
            //     if (!isErrorPageMissing(&status, errorPages, &fd)
            //             && isReadBufferOk(fd, &buffer)) {
            //         std::ostringstream bodyStream;
            //         bodyStream << buffer.str().size();
            //         res.addHeader("Content-Length", "bodyStream.str()");
            //         res.setBody("bodyStream.str()");
            //         close(fd);
            //         return res;
            //     }
            //     res.addHeader("Content-Length", "0");
            //     res.setBody("");
            //     if (!isReadBufferOk(fd, &buffer))
            //         res.setStatusCode(500);
            //     close(fd);
            //     return res;
            // }

            // static Response sendAdequateError(const int errorCode,
            //         const serverConfig::ServiceConfigErrorPages& errorPages,
            //         const std::string& connection) {
            //     int code = 0;
            //     switch (errorCode) {
            //         case ENOENT: // File not found
            //             // NGINX behavior: 404 Not Found
            //             code = 404;
            //             break;
            //         case EACCES: // Permission denied
            //         case EPERM:  // Operation not permitted
            //         case EISDIR: // It is a directory
            //             // NGINX behavior: 403 Forbidden
            //             code = 403;
            //             break;
            //         default:
            //             // Something weird happened (Disk full, IO error)
            //             std::cerr << "Delete failed: " << strerror(errno) << std::endl;
            //             code = 500;
            //     }
            //     return sendError(code, errorPages.getValue(), connection);
            // }

            // static std::string sendMsgError(const int statusCode,
            //         const std::map<int, std::string>& errorPages,
            //         const std::string& connection) {
            //     int status = statusCode;
            //     int fd = -1;
            //     std::stringstream  response;
            //     std::stringstream buffer;
            //     if (isErrorPageMissing(&status, errorPages, &fd)) {
            //         response << "HTTP/1.1 " << status << " "
            //        << http::HttpStatus::getReasonPhrase(status) << "\r\n"
            //        << "Content-Type: text/plain\r\n"
            //        << "Connection: " << connection << "\r\n"
            //         << "Content-Length: 0\r\n\r\n";
            //        return response.str();
            //     }
            //     if (isReadBufferOk(fd, &buffer)) {
            //         response << "HTTP/1.1 " << status << " "
            //         << http::HttpStatus::getReasonPhrase(status) << "\r\n"
            //         << "Content-Type: text/plain\r\n"
            //         << "Connection: " << connection << "\r\n"
            //         << "Content-Length: " << buffer.str().size()
            //         << "\r\n\r\n" << buffer.str();
            //         return response.str();
            //     }
            //     response << "HTTP/1.1 " << 500 << " "
            //        << http::HttpStatus::getReasonPhrase(status) << "\r\n"
            //        << "Content-Type: text/plain\r\n"
            //        << "Connection: " << connection << "\r\n"
            //         << "Content-Length: 0\r\n\r\n";
            //     return response.str();
            // }
        };
    }
}
#endif //WEBSERV_HTTPERROR_HPP