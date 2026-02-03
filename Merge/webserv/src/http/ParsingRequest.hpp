/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:29:01 by yroard            #+#    #+#             */
/*   Updated: 2026/01/28 14:45:50 by yroard           ###   ########.fr       */
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
			Buffer  m_rawData;
			std::string m_method;
			std::string  m_path;
			std::string  m_queryString;
			std::string  m_httpVersion;
			std::map<std::string, std::string> m_header;
			Buffer m_body;

			// Static because they don't touch member variables directly
			static std::string parseLineRequest(std::string* rawHeader,
				const std::string& s, tExceptError error);
			static std::string parseForQueryLineRequest(std::string *fullUri);
			static std::string trimHeader(const std::string& str);
			static std::map<std::string, std::string> parseForHeaderLineRequest(
				std::string &rawHeader);
			static std::string findEndHeader(Buffer rawData, unsigned* index);
			static Buffer fillBody(Buffer rawData, unsigned index);

		public:
			ParsingRequest();
			explicit ParsingRequest(const Buffer& rawData);
			ParsingRequest(const ParsingRequest &src);
			ParsingRequest &operator=(const ParsingRequest &src);
			~ParsingRequest();

			// Getters
			const std::string& getMethod() const;
			const std::string& getPath() const;
			const std::string& getQuery() const;
			const std::string& getHttpVersion() const;
			const std::string getHeaderInfo(const std::string& ToFind)const;
			const Buffer& getBody() const;

			static ParsingRequest parseRequest(Buffer &rawData);
			void printReqBody()const;
			void printRequest();
			void reset();
		};
	}
}

#endif
