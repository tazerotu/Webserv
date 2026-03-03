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
            invalid_file,
            cgi_exec_failure,
            invalid_cgi_extension,
            cgi_script_invalid,
            cgi_interpreter_invalid,
            invalid_default_file,
            writing_to_stringstream_failure,
            read_failure
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
                        full_msg = "pipe failed";
                        break;
                    case fork_error:
                        full_msg = "fork failed";
                        break;
                    case execve_error:
                        full_msg = "execve error";
                        break;
                    case invalid_file:
                        full_msg = "error in opening file";
                        break;
                    case cgi_exec_failure:
                        full_msg = "CGI Script execution failed";
                        break;
                    case invalid_cgi_extension:
                        full_msg = "File extension does not match CGI";
                        break;
                    case cgi_script_invalid:
                        full_msg = "CGI script not found or not readable";
                        break;
                    case cgi_interpreter_invalid:
                        full_msg = "CGI Interpreter not found";
                        break;
                    case invalid_default_file:
                        full_msg = "default file is missing";
                        break;
                    case writing_to_stringstream_failure:
                        full_msg = "Failed to write to stringstream";
                        break;
                    case read_failure:
                        full_msg = "read() failed";
                        break;
                    default:
                        full_msg = "http error";
                }
                return HttpError(status.getStatusCodeInt(), full_msg);
            }

            int getCode() const throw() {
                return m_code;
            }

            virtual const char* what() const throw() {
                return m_logMessage.empty() ? "HTTP Error" : m_logMessage.c_str();
            }

            static bool isErrorPageMissing(int* status,
                    const std::map<int, std::string>& errorPages,
                    int* fd){
                if (!HttpStatus::IsValidStatusCode(*status)) {
                    std::stringstream sStatus;
                    sStatus << *status;
                    Logger::messagesFilter(ERR,
                         "status code is invalid: ",
                        sStatus.str());
                    *status = 500;
                }
                std::map<int, std::string>::const_iterator it
                    = errorPages.find(*status);
                if (it != errorPages.end()) {
                    Logger::messagesFilter(DEBUG,
                        "it->second.c_str(): ",
                        it->second.c_str());
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
        };
    }
}
#endif //WEBSERV_HTTPERROR_HPP