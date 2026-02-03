//
// Created by yannou on 30/01/2026.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <string>
#include <map>
#include "HttpStatus.hpp"

namespace webserv {
    namespace http {
        class Response {
        private:
            int m_statusCode;
            std::string m_body;
            std::map<std::string, std::string> m_headers;

        public:
            Response() : m_statusCode(0), m_body(), m_headers(){}
            explicit Response(const int statusCode, const std::string& body,
                const std::map<std::string, std::string>& headers): m_statusCode(statusCode),
                m_body(body), m_headers(headers){}
            Response(const Response& other) : m_statusCode(other.m_statusCode),
                m_body(other.m_body), m_headers(other.m_headers){}
            ~Response(){}
            Response operator=(const Response& other){
                if (this != &other) {
                    m_statusCode = other.m_statusCode;
                    m_body = other.m_body;
                    m_headers = other.m_headers;
                }
                return *this;
            }

            void setStatusCode(const int statusCode) {
                m_statusCode = statusCode;
            }

            void setBody(const std::string& body) {
                m_body = body;
            }

            void addHeader(const std::string& key, const std::string& val) {
                m_headers[key] = val;
            }
            // Combines everything into the final HTTP string
            std::string httpString() {
                std::stringstream response;
                response << "HTTP/1.1 " << m_statusCode << " "
                    << HttpStatus::getReasonPhrase(m_statusCode) << "\r\n";
                for (
                    std::map<std::string, std::string>::iterator it = m_headers.begin(); it != m_headers.end(); ++it) {
                        response << it->first << ": " << it->second << "\r\n";
                }
                response << "\r\n" << m_body;
                return response.str();
            }


        };
    }
}
#endif //WEBSERV_RESPONSE_HPP