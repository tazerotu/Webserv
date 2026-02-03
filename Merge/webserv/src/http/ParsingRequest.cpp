#include <sstream>
#include <iostream>
#include "HttpError.hpp"
#include "ParsingRequest.hpp"



namespace webserv {
	namespace http {
		std::ostream &operator<<(std::ostream &o, Buffer &rhs) {
			Buffer::iterator itEnd = rhs.end();
			unsigned index = 0;
			for (Buffer::iterator it = rhs.begin(); it != itEnd; ++it,
					++index) {
				o << rhs[index];
			}
			return o;
		}

        std::string ParsingRequest::parseLineRequest(
        		std::string* rawHeader, const std::string & s,
        		tExceptError error) {
			const unsigned long pos = rawHeader->find_first_of(s);
			if (pos == std::string::npos)
				throw HttpError::create(HttpStatus(BadRequest), error);
			std::string returnString = rawHeader->substr(0, pos);
			std::string splitRawHeader = rawHeader->substr(pos + s.size(),
				std::string::npos);
			*rawHeader = splitRawHeader;
			return returnString;
		}

	    std::string ParsingRequest::parseForQueryLineRequest(
				std::string *fullUri){
			const unsigned long posQuery  = fullUri->find_first_of('?');
			if (posQuery == std::string::npos)
				return "";
			std::string returnString = fullUri->substr(posQuery + 1);
			std::string RawHeaderPath = fullUri->substr(0, posQuery);
			*fullUri = RawHeaderPath;
			return returnString;
		}

		std::string ParsingRequest::trimHeader(const std::string& str) {
			const char* whitespace = " \t\r\n";
			size_t first = str.find_first_not_of(whitespace);
			if (first == std::string::npos)
				return ""; // String is all whitespace
			size_t last = str.find_last_not_of(whitespace);
			return str.substr(first, (last - first + 1));
		}

		std::map<std::string, std::string> ParsingRequest::parseForHeaderLineRequest(
				std::string &rawHeader) {
			std::map<std::string, std::string> header;
			std::stringstream ss(rawHeader);
			std::string t;
			while (std::getline(ss, t, '\n')){
				const size_t colonPos = t.find_first_of(':');
				if (colonPos != std::string::npos) {
					std::string key = t.substr(0, colonPos);
					std::string value = t.substr(colonPos + 1);
					key = trimHeader(key);     
					value = trimHeader(value); 
					header[key] = value;
				}
				else
					return header;
			}
			return header;
		}

		std::string ParsingRequest::findEndHeader(Buffer rawData,
				unsigned* index) {
			std::string rawBuffer;
			Buffer::iterator it;
			Buffer::iterator itEnd = rawData.end();
			for (it = rawData.begin(); it != itEnd; ++it, ++(*index)) {
				rawBuffer += rawData[*index];
				if (*index > 3 && rawData[*index] == '\n'
					&& rawData[*index - 1] == '\r'
					&& rawData[*index - 2] == '\n'
					&& rawData[*index - 3] == '\r')
					return rawBuffer;
			}
			return rawBuffer;
		}

		Buffer ParsingRequest::fillBody(Buffer rawData, unsigned index) {
			// Safety check: ensure index is within bounds
            if (index >= rawData.size()) {
                // TO DO: Determine if this is an error or just an empty body
                // For now, return empty body
                return Buffer(); 
            }
			Buffer body;
			if (index > 3 && rawData[index] == '\n'
					&& rawData[index - 1] == '\r'
					&& rawData[index - 2] == '\n'
					&& rawData[index - 3] == '\r') {
				++index;
                if (index < rawData.size()) {
				    return Buffer(rawData.begin() + index,
				    	rawData.end());
                }
			}
			else {
				throw HttpError::create(HttpStatus(BadRequest),
					invalid_header);
			}    
			return Buffer(); // Return empty buffer if no body
		}

        ParsingRequest::ParsingRequest() : m_rawData(), m_method(), m_path(),
			m_queryString(), m_httpVersion(), m_header(), m_body() {
			std::cout << "ParsingRequest, default constructor called"
				<< std::endl;
		}

		ParsingRequest::ParsingRequest(const Buffer& rawData)
						: m_rawData(rawData), m_method(), m_path(),
						m_queryString(), m_httpVersion(), m_header(),
						m_body() {
			std::cout << "ParsingRequest, constructor called"
				<< std::endl;
			//First, find the end of the header
			unsigned index = 0;
			std::string rawBuffer = findEndHeader(rawData, &index);
			m_body = fillBody(rawData, index);
			size_t headerEnd = rawBuffer.find("\r\n\r\n");
			std::string rawHeader;
			rawHeader = rawBuffer.substr(0, headerEnd);
			m_method = parseLineRequest(&rawHeader, " ",
				invalid_method);
			std::string fullUri = parseLineRequest(&rawHeader,
				" ", invalid_path);
			m_queryString = parseForQueryLineRequest(&fullUri);
			m_path = fullUri;
			m_httpVersion = parseLineRequest(&rawHeader, "\r\n",
					invalid_httpVersion);
			m_header = parseForHeaderLineRequest(rawHeader);
			printRequest();
		}

		// Make Copy Constructor Public
		ParsingRequest::ParsingRequest(const ParsingRequest &src) {
			*this = src;
		}

		ParsingRequest & ParsingRequest::operator=(
				const ParsingRequest &src) {
			if (this != &src) {
				m_rawData = src.m_rawData;
				m_method = src.m_method;
				m_path = src.m_path;
				m_queryString = src.m_queryString;
				m_httpVersion = src.m_httpVersion;
				m_header = src.m_header;
				m_body = src.m_body;
			}
			return *this;
		}

		ParsingRequest::~ParsingRequest() {
		}

		ParsingRequest ParsingRequest::parseRequest(Buffer &rawData) {
			std::cout << "parseRequest" << std::endl;
			return ParsingRequest(rawData);
		}

		const std::string& ParsingRequest::getMethod() const {
			return m_method;
		}

		const std::string& ParsingRequest::getPath() const {
			return m_path;
		}

		const std::string& ParsingRequest::getQuery() const {
			return m_queryString;
		}

		const std::string& ParsingRequest::getHttpVersion() const {
			return m_httpVersion;
		}

		const std::string ParsingRequest::getHeaderInfo(
				const std::string& ToFind)const{
			std::map<std::string, std::string>::const_iterator it =
					m_header.find(ToFind);
				if (it != m_header.end()) {
					return it->second;
				}
				return "";
		}
		const Buffer& ParsingRequest::getBody() const {
			return m_body;
		}

		void ParsingRequest::printReqBody()const {
			for (unsigned i = 0; i < m_body.size(); ++i) {
				char c = m_body[i];
				if (c == '\r')
					std::cout << "\\r"; // Print literally "\r"
				else if (c == '\n')
					std::cout << "\\n\n"; // Print literally "\n" then a real newline
				else if (std::isprint(c))
					std::cout << c;
				else
					std::cout << "."; // Replace non-printables with dot
			}
		}

		void ParsingRequest::printRequest() {
			std::cout << "Method: " << m_method << std::endl; // e.g., "GET"
			std::cout << "Path: " << m_path << std::endl; // e.g., "/index.html"
			std::cout << "Query: " << m_queryString << std::endl; // e.g., "/index.html"
			std::cout << "Version: " << m_httpVersion << std::endl; // e.g., "HTTP/1.1"
			std::cout << "Header: " << std::endl;
			std::cout << "========== START HEADER =========="
				<< std::endl;
			for (std::map<std::string, std::string>::iterator itHeader =
						m_header.begin(); itHeader != m_header.end();
						++itHeader) {
				std::cout << itHeader->first << ": "
						<< itHeader->second << std::endl;
			}
			std::cout << "\n========== END HEADER =========="
				<< std::endl;
			std::cout << "Body: " << std::endl;
			std::cout << "========== START BODY ==========" << std::endl;
			for (unsigned i = 0; i < m_body.size(); ++i) {
				char c = m_body[i];
				if (c == '\r')
					std::cout << "\\r"; // Print literally "\r"
				else if (c == '\n')
					std::cout << "\\n\n"; // Print literally "\n" then a real newline
				else if (std::isprint(c))
					std::cout << c;
				else
					std::cout << "."; // Replace non-printables with dot
			}
			std::cout << "\n========== END BODY ==========" << std::endl;
		}

		void ParsingRequest::reset() {
			m_rawData.clear();
			m_method.erase();
			m_path.erase();
			m_queryString.erase();
			m_httpVersion.erase();
			m_header.clear();
			m_body.clear();
		}
	}
}
