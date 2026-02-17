//
// Created by yannou on 30/01/2026.
//

#pragma
#ifndef WEBSERV_CGIHANDLER_HPP
#define WEBSERV_CGIHANDLER_HPP
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib>
#include "HttpError.hpp"

namespace webserv {
    namespace http {
        class CgiHandler {
        private:
        CgiHandler(){}

        public:
            //Check file extension, file existence, interpreter, then
            //execution and response
            static Response processCGI(const ParsingRequest& req,
                    const serverConfig::routes::IServerConfigRoutes* targetRoute,
                    const std::string& path) {
                const std::string& cgiExtension
                    = targetRoute->getCGI().getCGIExtension();
                if (path.find(cgiExtension) == std::string::npos) {
                    throw HttpError::create(HttpStatus(NotFound),
                        invalid_cgi_extension);
                }
                if (access(path.c_str(), R_OK) == -1) {
                    throw HttpError::create(HttpStatus(NotFound),
                        cgi_script_invalid);
                }
                const std::string& cgiInterpreterPath
                    = targetRoute->getCGI().getCGIInterpreterPath();
                if (!isInterpreterInstalled(cgiInterpreterPath)) {
                    throw HttpError::create(HttpStatus(InternalServerError),
                        cgi_interpreter_invalid);
                }
				
				try {
					std::string cgiOutput = execute(path, cgiInterpreterPath, req);
					Response res;
					res.setStatusCode(200);
					size_t headerEnd = cgiOutput.find("\r\n\r\n");
					if (headerEnd != std::string::npos) {
						std::string headers = cgiOutput.substr(0, headerEnd);
						std::string body = cgiOutput.substr(headerEnd + 4);
						res.setBody(body);
						std::map<std::string, std::string> headerMap;
						headerMap =
							ParsingRequest::parseForHeaderLineRequest(headers);
						std::map<std::string, std::string>::iterator it;
						Logger::MessagesFilter(DEBUG,
							"Into php file, header: ","");
						for (it = headerMap.begin(); it != headerMap.end(); ++it){
							Logger::MessagesFilter(DEBUG,
							"Into loop, header: ","");
							std::cout << it->first << ": " << it->second << std::endl;
						}
						Logger::MessagesFilter(DEBUG,
							"Exit loop, header","");
						if (!req.getHeaderInfo("Connection").empty())
							res.addHeader("Connection",
								req.getHeaderInfo("Connection"));
						res.addHeader("Content-Type", "text/html");
					}
					else 
						res.setBody(cgiOutput);
					return res;
				}
				catch (const HttpError& e) {
					HttpError::create(HttpStatus(InternalServerError), cgi_exec_failure);
				}
					
				// = execute(path, cgiInterpreterPath, req);
				Response resp;
				resp.setStatusCode(500);
				return(resp);
            }

            //  checks if the file exists AND has execute permissions
            static bool isInterpreterInstalled(const std::string &path) {
                if (access(path.c_str(), X_OK) == 0)
                    return true;
                return false;
            }

        private:
            // Helper to clean up memory in child process
            static void freeEnv(char** env) {
                for (int i = 0; env[i] != NULL; ++i) {
                    delete[] env[i];
                }
                delete[] env;
            }

            // Helpers for setting up env, etc.
            static char** setUpEnvParameters(const http::ParsingRequest& req,
                    const std::string& scriptPath) {
                std::vector<std::string> envStr;
                envStr.push_back("REQUEST_METHOD=" + req.getMethod());
                envStr.push_back("REQUEST_URI=" + req.getFullUri());
                envStr.push_back("SCRIPT_FILENAME=" + scriptPath);
				envStr.push_back("SCRIPT_NAME=" + req.getUri());
                envStr.push_back("QUERY_STRING=" + req.getQuery());
                envStr.push_back("SERVER_PROTOCOL=" + req.getHttpVersion());
                envStr.push_back("GATEWAY_INTERFACE=CGI/1.1");
                envStr.push_back("REDIRECT_STATUS=200");
                if (!req.getHeaderInfo("Content-Length").empty())
                    envStr.push_back("CONTENT_LENGTH=" 
						+ req.getHeaderInfo("Content-Length"));
                if (!req.getHeaderInfo("Content-Type").empty())
                    envStr.push_back("CONTENT_TYPE=" 
						+ req.getHeaderInfo("Content-Type"));
				if (!req.getHeaderInfo("Host").empty())
                    envStr.push_back("HTTP_HOST=" 
						+ req.getHeaderInfo("Host"));
				if (!req.getHeaderInfo("User-Agent").empty())
                    envStr.push_back("HTTP_USER_AGENT="
                        + req.getHeaderInfo("User-Agent"));
				if (!req.getHeaderInfo("Accept").empty())
                    envStr.push_back("HTTP_ACCEPT="
                        + req.getHeaderInfo("Accept"));
				if (!req.getHeaderInfo("Accept-Language").empty())
                    envStr.push_back("HTTP_ACCEPT_LANGUAGE="
                        + req.getHeaderInfo("Accept-Language"));
				if (!req.getHeaderInfo("Accept-Encoding").empty())
                    envStr.push_back("HTTP_ACCEPT_ENCODING="
                        + req.getHeaderInfo("Accept-Encoding"));
				if (!req.getHeaderInfo("Connection").empty())
                    envStr.push_back("HTTP_CONNECTION="
                        + req.getHeaderInfo("Connection"));
                // Convert to char**
                char** envP = new char*[envStr.size() + 1];
                for (size_t i = 0; i < envStr.size(); ++i) {
                    envP[i] = new char[envStr[i].size() + 1];
                    std::strcpy(envP[i], envStr[i].c_str());
                }
                envP[envStr.size()] = NULL;
                return envP;
            }

            static std::string execute(const std::string& scriptPath,
                    const std::string& interpreter,
                    const ParsingRequest& req) {
                int pipe_out[2];
                if (pipe(pipe_out) == -1) {
                    throw HttpError::create(HttpStatus(InternalServerError), pipe_error);
                }
                int pipe_in[2];
                if (pipe(pipe_in) == -1) {
                    close(pipe_out[0]);
                    close(pipe_out[1]);
                    throw HttpError::create(HttpStatus(InternalServerError), pipe_error);
                }
                pid_t pid = fork();
                if (pid == -1) {
                    close(pipe_out[0]); close(pipe_out[1]);
                    close(pipe_in[0]); close(pipe_in[1]);
                    throw HttpError::create(HttpStatus(InternalServerError),
                        fork_error);
                }
                // --- CHILD PROCESS (Executes PHP) ---
                if (pid == 0) {
                    dup2(pipe_out[1], STDOUT_FILENO);
                    close(pipe_out[0]);
                    close(pipe_out[1]);
                    dup2(pipe_in[0], STDIN_FILENO);
                    close(pipe_in[0]);
                    close(pipe_in[1]);
                    // argv[0] = interpreter, argv[1] = script, argv[2] = NULL
                    char *argv[] = {
                        const_cast<char*>(interpreter.c_str()),
                        const_cast<char*>(scriptPath.c_str()),
                        NULL
                    };
                    char** envP = setUpEnvParameters(req, scriptPath);
                    execve(interpreter.c_str(), argv, envP);
                    std::cerr << "Execve failed" << std::endl;
                    freeEnv(envP);
                    exit(1);
                }
                // --- PARENT PROCESS (Reads Output) ---
                close(pipe_out[1]);
                close(pipe_in[0]);
                const Buffer& body = req.getBody();
                if (!body.empty()) {
                    write(pipe_in[1], &body[0], body.size());
                }
                close(pipe_in[1]);
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    // close(pipe_fd[0]);
                    close(pipe_out[0]);
                    throw HttpError::create(HttpStatus(InternalServerError),
                        cgi_exec_failure);
                }
                char buffer[4096];
                std::string cgiOutput;
                ssize_t bytesRead;
                while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer))) > 0) {
                    cgiOutput.append(buffer, bytesRead);
                }
                close(pipe_out[0]);
                return cgiOutput;
            }
        };
    }
}

#endif //WEBSERV_CGIHANDLER_HPP