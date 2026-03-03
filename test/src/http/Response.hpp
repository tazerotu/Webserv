/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:58:10 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 13:52:30 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <string>
#include <map>

#include "../ConvUtils.hpp"
#include "HttpStatus.hpp"
#include "../Logger.hpp"

namespace webserv {
    namespace http {
        class Response {
        private:
            int m_statusCode;
            std::string m_body;
            std::map<std::string, std::string> m_headers;

        public:
            Response() : m_statusCode(0), m_body(), m_headers(){
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

			int getStatusCode(){
                return m_statusCode;
            }

            void setStatusCode(const int statusCode) {
                m_statusCode = statusCode;
            }

            void setBody(const std::string& body) {
                m_body = body;
                addHeader("Content-Length",
                    ConvUtils::sizeTToStr(body.size()));
            }

            void addHeader(const std::string& key, const std::string& val) {
                m_headers[key] = val;
            }
            
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
                if (it == m_headers.end())
                    return false;
                Logger::messagesFilter(INFO,
                    "m_headers.find(\'Connection\'): ",
                    it->second);
                if (it != m_headers.end() && (it->second == "close"
                        || it->second == "Close"
                        || it->second == "CLOSE")) {
                    return true;
                }
                return false;
            }

        };
    }
}
#endif //WEBSERV_RESPONSE_HPP
