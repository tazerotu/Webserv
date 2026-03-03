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
				return "";
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

        ParsingRequest::ParsingRequest() : m_rawData(), m_method(),
			m_fullUri(), m_uri(), m_path(), m_queryString(), m_httpVersion(),
			m_header(), m_body(), m_headerParsed(false), m_expectedBodySize(0) {
			Logger::messagesFilter(INFO,
				"ParsingRequest, default constructor called",
				"");
		}

		ParsingRequest::ParsingRequest(const Buffer& rawData)
				: m_rawData(rawData), m_method(), m_fullUri(), m_uri(),
				m_path(), m_queryString(), m_httpVersion(), m_header(),
				m_body(), m_headerParsed(false), m_expectedBodySize(0) {
		}

		ParsingRequest::ParsingRequest(const ParsingRequest &src) {
			*this = src;
		}

		ParsingRequest & ParsingRequest::operator=(
				const ParsingRequest &src) {
			if (this != &src) {
				m_rawData = src.m_rawData;
				m_method = src.m_method;
				m_fullUri = src.m_fullUri;
				m_uri = src.m_uri;
				m_path = src.m_path;
				m_queryString = src.m_queryString;
				m_httpVersion = src.m_httpVersion;
				m_header = src.m_header;
				m_body = src.m_body;
				m_headerParsed = src.m_headerParsed;
				m_expectedBodySize = src.m_expectedBodySize;
			}
			return *this;
		}

		ParsingRequest::~ParsingRequest() {
		}

		ParsingRequest ParsingRequest::parseRequest(Buffer &rawData) {
			Logger::messagesFilter(DEBUG,
	"parseRequest, constructor called","");
			return ParsingRequest(rawData);
		}

		const std::string& ParsingRequest::getMethod() const {
			return m_method;
		}

		const std::string& ParsingRequest::getFullUri()const{
			return m_fullUri;
		}

		const std::string& ParsingRequest::getUri()const{
			return m_uri;
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

		size_t ParsingRequest::getExpectedSize()const {
			return m_expectedBodySize;
		}

		const std::string ParsingRequest::getRemainingData() const {
			return m_rawData.getBufferStr();
		}

		void ParsingRequest::printReqBody()const {
			if (m_body.size() == 0) {
				Logger::messagesFilter(INFO, "No body",
					"");
				return;
			}

			Logger::messagesFilter(INFO,
				"========== START BODY ==========", "");
			for (unsigned i = 0; i < m_body.size(); ++i) {
				char c = m_body[i];
				if (c == '\r')
					std::cout << "\\r";
				else if (c == '\n')
					std::cout << "\\n\n";
				else if (std::isprint(c))
					std::cout << c;
				else
					std::cout << ".";
			}
			Logger::messagesFilter(INFO,
	 "=========== END BODY ==========", "");
		}

		void ParsingRequest::printRequest() {
			std::cout << "Method: " << m_method << std::endl;
			std::cout << "Full uri: " << m_fullUri << std::endl;
			std::cout << "Uri: " << m_uri << std::endl;
			std::cout << "Path: " << m_path << std::endl;
			std::cout << "Query: " << m_queryString << std::endl;
			std::cout << "Version: " << m_httpVersion << std::endl;
			Logger::messagesFilter(INFO,
							"========== START HEADER ==========", "");
			for (std::map<std::string, std::string>::iterator itHeader =
						m_header.begin(); itHeader != m_header.end();
						++itHeader) {
				std::cout << itHeader->first << ": "
						<< itHeader->second << std::endl;
			}
			Logger::messagesFilter(INFO,
	"=========== END HEADER ==========", "");
			printReqBody();
		}

		void ParsingRequest::reset() {
			m_rawData.clear();
			m_method.erase();
			m_path.erase();
			m_queryString.erase();
			m_httpVersion.erase();
			m_header.clear();
			m_body.clear();
			m_headerParsed = false;
			m_expectedBodySize = 0;
		}

void ParsingRequest::parseHeader(std::string& rawHeader) {
			m_method = parseLineRequest(&rawHeader, " ",
				invalid_method);
			std::string fullUri = parseLineRequest(&rawHeader,
				" ", invalid_path);
			m_fullUri = fullUri;
			size_t pos = fullUri.find("?");
			if (pos == std::string::npos)
				m_uri = fullUri;
			else
				m_uri = fullUri.substr(0, pos);
			m_queryString = parseForQueryLineRequest(&fullUri);
			m_path = fullUri;
			m_httpVersion = parseLineRequest(&rawHeader, "\r\n",
					invalid_httpVersion);
			m_header = parseForHeaderLineRequest(rawHeader);
			std::stringstream ss;
			ss << getHeaderInfo("Content-Length");
			ss >> m_expectedBodySize;
			std::cout << "m_expectedBodySize=" << m_expectedBodySize << std::endl;
		}

		//parse only if end of the header found
		void ParsingRequest::parse(){
			std::string bufferStr = m_rawData.getBufferStr();
			if (!m_headerParsed){
				size_t posEndHeader = bufferStr.find("\r\n\r\n");
				if (posEndHeader == std::string::npos)
					return;
				std::string rawHeader =
					bufferStr.substr(0, posEndHeader + 4);
				parseHeader(rawHeader);
				bufferStr.erase(0, posEndHeader + 4);
				m_headerParsed = true;
				m_rawData.clear();
				if (bufferStr.size()) {
					m_rawData.append(bufferStr);
					Logger::messagesFilter(DEBUG,
				"m_rawData, body only: ",
				m_rawData.getBufferStr());
				}
			}
			if (m_headerParsed && m_expectedBodySize > 0) {
				size_t bytesNeeded = m_expectedBodySize - m_body.size();
				size_t bytesAvailable = bufferStr.size();

				size_t toCopy = std::min(bytesNeeded, bytesAvailable);
				if (toCopy > 0) {
					m_body.insert(bufferStr, 0, toCopy);
					m_rawData.clear();
					if (bytesAvailable > toCopy) {
						m_rawData.append(bufferStr.substr(toCopy));
					}
				}
			}
		}

		void ParsingRequest::appendData(const std::string& chunk) {
			m_rawData.append(chunk);
			parse();
		}

		bool ParsingRequest::isComplete() {
			parse();
			if (m_headerParsed && m_body.size() == m_expectedBodySize)
				return true;
			return false;
		}
	}
}
