/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:58:10 by yroard            #+#    #+#             */
/*   Updated: 2026/02/04 09:33:08 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
            Response() : m_statusCode(0), m_body(), m_headers(){
                addHeader("Content-Length", "text/plain");
            }
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
                // Automatically set the Content-Length header
                std::stringstream ss;
                ss << m_body.size();
                addHeader("Content-Length", ss.str());
            }

            void addHeader(const std::string& key, const std::string& val) {
                m_headers[key] = val;
            }
            
            // Combines everything into the final HTTP string
            std::string httpString() const{
                std::stringstream response;
                response << "HTTP/1.1 " << m_statusCode << " "
                    << HttpStatus::getReasonPhrase(m_statusCode) << "\r\n";
                std::map<std::string, std::string>::const_iterator it;
                for (it = m_headers.begin(); it != m_headers.end(); ++it) {
                        response << it->first << ": " << it->second << "\r\n";
                }
                response << "\r\n" << m_body;
                return response.str();
            }

            bool isConnectionToBeClosed(){
                std::map<std::string, std::string>::iterator it;
                it = m_headers.find("Connection");
                if (it != m_headers.end() && (it->second.find("close") || it->second.find("Close")
                        || it->second.find("CLOSE"))) 
                    return true;
                return false;
            }

        };
    }
}
#endif //WEBSERV_RESPONSE_HPP