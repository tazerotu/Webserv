//
// Created by yannou on 30/01/2026.
//

#ifndef WEBSERV_CGIHANDLER_HPP
#define WEBSERV_CGIHANDLER_HPP
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "HttpError.hpp"

namespace webserv {
    namespace http {
        class CgiHandler {
        private:


        public:
            static std::string execute(const std::string& scriptPath,
                                const std::string& interpreter,
                                const std::map<int, std::string>& errorPages,
                                const std::string& connection) {
                int pipe_fd[2];
                if (pipe(pipe_fd) == -1) {
                    HttpError::create(HttpStatus(InternalServerError),
                        pipe_error);
                    return ErrorPageGenerator::generateMsg(500,
                        errorPages);
                }
                pid_t pid = fork();
                if (pid == -1) {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    HttpError::create(HttpStatus(InternalServerError),
                        fork_error);
                    return ErrorPageGenerator::generateMsg(500,
                        errorPages);
                }
                // --- CHILD PROCESS (Executes PHP) ---
                if (pid == 0) {
                    // 1. Redirect Standard Output to the Pipe
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    // 2. Prepare Arguments
                    // argv[0] = interpreter, argv[1] = script, argv[2] = NULL
                    char *argv[] = {
                        const_cast<char*>(interpreter.c_str()),
                        const_cast<char*>(scriptPath.c_str()),
                        NULL
                    };
                    // 3. Prepare Environment Variables (Crucial for PHP-CGI)
                    std::vector<char*> envP = setUpEnvParameters(scriptPath);
                    // 4. Execute
                    execve(interpreter.c_str(), argv, envP.data());
                    // If execve returns, it failed
                    std::cerr << "Execve failed" << std::endl;
                    HttpError::create(HttpStatus(InternalServerError),
                        execve_error);
                    exit(1);
                }
                // --- PARENT PROCESS (Reads Output) ---
                close(pipe_fd[1]); // Close write end
                int status;
                waitpid(pid, &status, 0); // Wait for PHP to finish
                // Read the output from the pipe
                char buffer[4096];
                std::string cgiOutput;
                ssize_t bytesRead;
                while ((bytesRead = read(pipe_fd[0], buffer,
                        sizeof(buffer))) > 0) {
                    cgiOutput.append(buffer, bytesRead);
                }
                close(pipe_fd[0]);
                return cgiOutput;
            }
            static std::string buildCGIResponse(
                            const std::string& scriptPath,
                            const std::string& cgiInterpreterPath,
                            const std::map<int, std::string>& errorPages,
                            const std::string& connection) {
                const std::string& cgiOutput =
                    execute(scriptPath, cgiInterpreterPath,
                        errorPages, connection);
                size_t pos = cgiOutput.find_first_of("\r\n\r\n", 0);
                std::string body;
                if (pos != std::string::npos)
                    body = cgiOutput.substr(pos + 4);
                else
                    body = cgiOutput;
                return body;
            }

            static Response processCGI(const http::ParsingRequest& req,
                    const serverConfig::ServerConfig& conf,
                    const serverConfig::routes::IServerConfigRoutes* targetRoute,
                    const std::string& path) {
                CgiHandler cgi;
                const std::string& cgiExtension
                    = targetRoute->getCGI().getCGIExtension();
                if (path.find(cgiExtension) != std::string::npos) {
                    int fd = open(path.c_str(), O_RDONLY);
                    if (fd == -1) {
                        // Add perror to see WHY it failed
                        //(usually Permission or No Entry)
                        std::perror("***Open failed");
                        HttpError::create(HttpStatus(NotFound),
                            invalid_file);
                        return ErrorPageGenerator::generate(404,
                        conf.getErrorPages().getValue());
                    }
                    close(fd);
                    //-> check if interpreter exists (else 500)
                    const std::string& cgiInterpreterPath
                        = targetRoute->getCGI().getCGIInterpreterPath();
                    if (!http::CgiHandler::isInterpreterInstalled(cgiInterpreterPath)) {
                        return ErrorPageGenerator::generate(500,
                            conf.getErrorPages().getValue());
                    }
                    // CGI SUCCESS (Placeholder)
                    // You need to actually execute CGI here later
                    std::string body = buildCGIResponse(path, cgiInterpreterPath,
                    conf.getErrorPages().getValue(),
                    req.getHeaderInfo("Connection"));
                    std::stringstream ss;
                    ss << body.size();
                    Response res;
                    res.setStatusCode(200);
                    res.addHeader("Connection", req.getHeaderInfo("Connection"));
                    res.addHeader("Content-Length", ss.str());
                    res.setBody(body);
                    return res;
                }
                return ErrorPageGenerator::generate(404,
                    conf.getErrorPages().getValue());
            }

            static bool isInterpreterInstalled(const std::string &path) {
                // X_OK checks if the file exists AND has execute permissions
                if (access(path.c_str(), X_OK) == 0) {
                    return true;
                }
                return false;
            }

        private:
            // Helpers for setting up env, etc.
            static std::vector<char*> setUpEnvParameters(const std::string& scriptPath) {
                // You usually convert the Request headers here.
                // For basic testing, these are the minimums:
                std::vector<std::string> envStr;
                envStr.push_back("REQUEST_METHOD=GET");
                envStr.push_back("SCRIPT_FILENAME=" + scriptPath);
                envStr.push_back("REDIRECT_STATUS=200"); // PHP-CGI needs this to run directly
                envStr.push_back("SERVER_PROTOCOL=HTTP/1.1");
                envStr.push_back("GATEWAY_INTERFACE=CGI/1.1");
                // Convert to char**
                std::vector<char*> envP;
                for (size_t i = 0; i < envStr.size(); ++i) {
                    envP.push_back(const_cast<char*>(envStr[i].c_str()));
                }
                envP.push_back(NULL);
                return envP;
            }
        };
    }
}

#endif //WEBSERV_CGIHANDLER_HPP