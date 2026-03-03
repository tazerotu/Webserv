//
// Created by yannou on 10/02/2026.
//

#ifndef WEBSERV_CONVUTILS_HPP
#define WEBSERV_CONVUTILS_HPP

#include <iosfwd>
#include <sstream>
#include <sys/types.h>

namespace webserv {
    class ConvUtils {
    public:
        static std::string sizeTToStr(size_t value) {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        static std::string ssizeToStr(ssize_t value) {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        static std::string intToStr(int value) {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        static std::string uShortToStr(unsigned short value) {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
        static long cStringToLong(const std::string& str) {
            std::stringstream ss;
            ss << str;
            long value;
            ss >> value;
            return value;
        }
        static long stringToLong(std::string& str) {
            std::stringstream ss;
            ss << str;
            long value;
            ss >> value;
            return value;
        }
        static size_t cStringToSizeT(const std::string& str) {
            std::stringstream ss;
            ss << str;
            size_t value;
            ss >> value;
            return value;
        }
        static size_t stringToSizeT(std::string& str) {
            std::stringstream ss;
            ss << str;
            size_t value;
            ss >> value;
            return value;
        }
        static int cStringToInt(const std::string& str) {
            std::stringstream ss;
            ss << str;
            int value;
            ss >> value;
            return value;
        }
        static int stringToInt(std::string& str) {
            std::stringstream ss;
            ss << str;
            int value;
            ss >> value;
            return value;
        }
    };
}


#endif //WEBSERV_CONVUTILS_HPP