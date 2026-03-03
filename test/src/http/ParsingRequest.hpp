/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:29:01 by yroard            #+#    #+#             */
/*   Updated: 2026/02/11 08:54:57 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGREQUEST_HPP
#define PARSINGREQUEST_HPP

#include <string>
#include <map>
#include "../Buffer.hpp"
#include "ErrorPageGenerator.hpp"
#include "HttpError.hpp"


namespace webserv {
	namespace http {
		std::ostream &operator<<(std::ostream &o, Buffer &rhs);

		class ParsingRequest {
		private:
			Buffer		m_rawData;
			std::string	m_method;
			std::string	m_fullUri;
			std::string	m_uri;
			std::string	m_path;
			std::string	m_queryString;
			std::string	m_httpVersion;
			std::map<std::string, std::string> m_header;
			Buffer m_body;
			bool m_headerParsed;
    		size_t m_expectedBodySize;

			static std::string parseLineRequest(std::string* rawHeader,
				const std::string& s, tExceptError error);
			static std::string parseForQueryLineRequest(std::string *fullUri);
			static std::string trimHeader(const std::string& str);
			static std::string findEndHeader(Buffer rawData, unsigned* index);
			void parseHeader(std::string& rawHeader);
			void parse();

		public:
			ParsingRequest();
			explicit ParsingRequest(const Buffer& rawData);
			ParsingRequest(const ParsingRequest &src);
			ParsingRequest &operator=(const ParsingRequest &src);
			~ParsingRequest();

			// Getters
			const std::string& getMethod() const;
			const std::string& getFullUri()const;
			const std::string& getUri()const;
			const std::string& getPath() const;
			const std::string& getQuery() const;
			const std::string& getHttpVersion() const;
			const std::string getHeaderInfo(const std::string& ToFind)const;
			const Buffer& getBody() const;
			size_t getExpectedSize() const;
			const std::string getRemainingData() const;
			static std::map<std::string, std::string> parseForHeaderLineRequest(
							std::string &rawHeader);
			static ParsingRequest parseRequest(Buffer &rawData);
			void printReqBody()const;
			void printRequest();
			void reset();

			
			void appendData(const std::string& chunk);
			bool isComplete();
		};
	}
}

#endif
