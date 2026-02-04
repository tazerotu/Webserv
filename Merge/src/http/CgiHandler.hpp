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
        CgiHandler(){}

        public:
            static Response processCGI(const ParsingRequest& req,
                    const serverConfig::routes::IServerConfigRoutes* targetRoute,
                    const std::string& path) {
                // 1. Validation Logic
                const std::string& cgiExtension
                    = targetRoute->getCGI().getCGIExtension();
                // Verify Extension
                if (path.find(cgiExtension) == std::string::npos) {
                    throw HttpError::create(HttpStatus(NotFound),
                        invalid_cgi_extension);
                }
                // Verify File Existence
                if (access(path.c_str(), R_OK) == -1) {
                    throw HttpError::create(HttpStatus(NotFound),
                        cgi_script_invalid);
                }
                // Verify Interpreter
                const std::string& cgiInterpreterPath
                    = targetRoute->getCGI().getCGIInterpreterPath();
                if (!isInterpreterInstalled(cgiInterpreterPath)) {
                    throw HttpError::create(HttpStatus(InternalServerError),
                        cgi_interpreter_invalid);
                }
                // 2. Execution Logic
                // We pass the 'req' to allow dynamic environment setup
                std::string cgiOutput = execute(path, cgiInterpreterPath,
                        req);
                // 3. Response Construction
                Response res;
                res.setStatusCode(200);
                size_t headerEnd = cgiOutput.find("\r\n\r\n");
                if (headerEnd != std::string::npos) {
                    std::string headers = cgiOutput.substr(0, headerEnd);
                    std::string body = cgiOutput.substr(headerEnd + 4);
                    res.setBody(body);
                    std::map<std::string, std::string> headerMap;
			        headerMap = ParsingRequest::parseForHeaderLineRequest(headers);
                    std::map<std::string, std::string>::iterator it;
                    std::cout << "Into php file, header: " << std::endl;
                    for (it = headerMap.begin(); it != headerMap.end(); ++it){
                        std::cout << it->first << ": " << it->second << std::endl;
                    } 
                    if (!req.getHeaderInfo("Connection").empty())
                        res.addHeader("Connection", req.getHeaderInfo("Connection"));

                    
                    // TODO: You should actually parse 'headers' string and add them to 'res'
                    // For now, minimal implementation:
                    res.addHeader("Content-Type", "text/html");
                }
                else 
                    res.setBody(cgiOutput);
            return res;

            }

            static bool isInterpreterInstalled(const std::string &path) {
                //  checks if the file exists AND has execute permissions
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
                // You usually convert the Request headers here.
                // For basic testing, these are the minimums:
                std::vector<std::string> envStr;
                envStr.push_back("REQUEST_METHOD=" + req.getMethod());
                envStr.push_back("SCRIPT_FILENAME=" + scriptPath);
                envStr.push_back("SERVER_PROTOCOL=" + req.getHttpVersion());
                envStr.push_back("GATEWAY_INTERFACE=CGI/1.1");
                envStr.push_back("REDIRECT_STATUS=200"); // PHP-CGI needs
                //this to run directly
                // Handle Content-Length/Type for POST
                if (!req.getHeaderInfo("Content-Length").empty())
                    envStr.push_back("CONTENT_LENGTH="
                        + req.getHeaderInfo("Content-Length"));
                if (!req.getHeaderInfo("Content-Type").empty())
                    envStr.push_back("CONTENT_TYPE="
                        + req.getHeaderInfo("Content-Type"));
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
                // Pipe 1: Child -> Parent (Output/Response)
                int pipe_out[2];
                if (pipe(pipe_out) == -1) {
                    throw HttpError::create(HttpStatus(InternalServerError), pipe_error);
                }
                // Pipe 2: Parent -> Child (Input/Body)
                int pipe_in[2];
                if (pipe(pipe_in) == -1) {
                    close(pipe_out[0]);
                    close(pipe_out[1]);
                    throw HttpError::create(HttpStatus(InternalServerError), pipe_error);
                }
                // int pipe_fd[2];
                // if (pipe(pipe_fd) == -1) {
                //     throw HttpError::create(HttpStatus(InternalServerError),
                //         pipe_error);
                // }
                pid_t pid = fork();
                if (pid == -1) {
                    close(pipe_out[0]); close(pipe_out[1]);
                    close(pipe_in[0]); close(pipe_in[1]);
                    throw HttpError::create(HttpStatus(InternalServerError),
                        fork_error);
                }
                // --- CHILD PROCESS (Executes PHP) ---
                if (pid == 0) {
                    // // 1. Redirect Standard Output to the Pipe
                    // dup2(pipe_fd[1], STDOUT_FILENO);
                    // close(pipe_fd[0]);
                    // close(pipe_fd[1]);
                    // 1. Redirect Output (Stdout -> pipe_out)
                    dup2(pipe_out[1], STDOUT_FILENO);
                    close(pipe_out[0]);
                    close(pipe_out[1]);

                    // 2. Redirect Input (pipe_in -> Stdin)
                    dup2(pipe_in[0], STDIN_FILENO);
                    close(pipe_in[0]);
                    close(pipe_in[1]);
                    // 3. Prepare Arguments
                    // argv[0] = interpreter, argv[1] = script, argv[2] = NULL
                    char *argv[] = {
                        const_cast<char*>(interpreter.c_str()),
                        const_cast<char*>(scriptPath.c_str()),
                        NULL
                    };
                    // 3. Prepare Environment Variables (Crucial for PHP-CGI)
                    char** envP = setUpEnvParameters(req, scriptPath);
                    // 4. Execute
                    execve(interpreter.c_str(), argv, envP);
                    // If execve returns, it failed
                    std::cerr << "Execve failed" << std::endl;
                    freeEnv(envP);
                    exit(1);
                }
                // --- PARENT PROCESS (Reads Output) ---
                // close(pipe_fd[1]); // Close write end
                // 1. Close unused ends
                close(pipe_out[1]); // We don't write to output
                close(pipe_in[0]);  // We don't read from input
                // 2. Write the Request Body to the Child
                // Assuming req.getBody() returns a Buffer or string
                // You might need to adjust .getBufferStr() depending on your Buffer class 
                //implementation
                const std::vector<char>& body = req.getBody(); 
                if (!body.empty()) {
                    write(pipe_in[1], &body[0], body.size());
                }
                
                // 3. Close the write-end so Child gets EOF (or stops reading after Content-Length)
                close(pipe_in[1]);
                // 4. Wait for Child
                int status;
                waitpid(pid, &status, 0); // Wait for PHP to finish
                // Check if Child exited correctly
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    // close(pipe_fd[0]);
                    close(pipe_out[0]);
                    throw HttpError::create(HttpStatus(InternalServerError),
                        cgi_exec_failure);
                }
                // Read the output from the pipe
                char buffer[4096];
                std::string cgiOutput;
                ssize_t bytesRead;
                while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer))) > 0) {
                    cgiOutput.append(buffer, bytesRead);
                }
                close(pipe_out[0]);
                // while ((bytesRead = read(pipe_fd[0], buffer,
                //         sizeof(buffer))) > 0) {
                //     cgiOutput.append(buffer, bytesRead);
                // }
                // close(pipe_fd[0]);
                return cgiOutput;
            }
        };
    }
}

#endif //WEBSERV_CGIHANDLER_HPP