//
// Created by yannou on 30/01/2026.
//

#ifndef WEBSERV_MIMETYPES_HPP
#define WEBSERV_MIMETYPES_HPP

#include <string>

namespace webserv {
    namespace http {
        class MimeTypes {
        public:
            static std::string getType(const std::string& path) {
                if (path.rfind(".html") != std::string::npos)
                    return "text/html";
                if (path.rfind(".css") != std::string::npos)
                    return "text/css";
                if (path.rfind(".js") != std::string::npos)
                    return "application/javascript";
                if (path.rfind(".jpg") != std::string::npos ||
                        path.rfind(".jpeg") != std::string::npos)
                    return "image/jpeg";
                if (path.rfind(".png") != std::string::npos)
                    return "image/png";
                if (path.rfind(".gif") != std::string::npos)
                    return "image/gif";
                if (path.rfind(".ico") != std::string::npos)
                    return "image/x-icon";
                return "text/plain"; // Default
            }
        };
    }
}
#endif //WEBSERV_MIMETYPES_HPP