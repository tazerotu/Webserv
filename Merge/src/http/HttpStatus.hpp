/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:28:34 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:39:54 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUS_HPP
# define HTTPSTATUS_HPP

#include <string>
#include "../Webserv.hpp"

namespace webserv {
    namespace http {
        // Define HTTP status codes as an enum
        class HttpStatus {
        private:
            StatusCode m_status;
        public:
            // Constructor
            explicit HttpStatus(StatusCode status) : m_status(status) {
            }

            // Get the status code
            StatusCode getStatusCode() const {
                return m_status;
            }

            // Get the status code as an integer
            int getStatusCodeInt() const {
                return static_cast<int>(m_status);
            }

            // Get the reason phrase for the status code
            static std::string getReasonPhrase(int code) {
                switch (code) {
                    // 1xx Informational
                    case 100: return "Continue";
                    case 101: return "Switching Protocols";
                    case 102: return "Processing";
                    case 103: return "Early Hints";

                        // 2xx Success
                    case 200: return "OK";
                    case 201: return "Created";
                    case 202: return "Accepted";
                    case 203: return "Non-Authoritative Information";
                    case 204: return "No Content";
                    case 205: return "Reset Content";
                    case 206: return "Partial Content";
                    case 207: return "Multi-Status";
                    case 208: return "Already Reported";
                    case 226: return "IM Used";

                        // 3xx Redirection
                    case 300: return "Multiple Choices";
                    case 301: return "Moved Permanently";
                    case 302: return "Found";
                    case 303: return "See Other";
                    case 304: return "Not Modified";
                    case 305: return "Use Proxy";
                    case 307: return "Temporary Redirect";
                    case 308: return "Permanent Redirect";

                        // 4xx Client Error
                    case 400: return "Bad Request";
                    case 401: return "Unauthorized";
                    case 402: return "Payment Required";
                    case 403: return "Forbidden";
                    case 404: return "Not Found";
                    case 405: return "Method Not Allowed";
                    case 406: return "Not Acceptable";
                    case 407: return "Proxy Authentication Required";
                    case 408: return "Request Timeout";
                    case 409: return "Conflict";
                    case 410: return "Gone";
                    case 411: return "Length Required";
                    case 412: return "Precondition Failed";
                    case 413: return "Payload Too Large";
                    case 414: return "URI Too Long";
                    case 415: return "Unsupported Media Type";
                    case 416: return "Range Not Satisfiable";
                    case 417: return "Expectation Failed";
                    case 418: return "I'm a teapot";
                    case 421: return "Misdirected Request";
                    case 422: return "Unprocessable Entity";
                    case 423: return "Locked";
                    case 424: return "Failed Dependency";
                    case 425: return "Too Early";
                    case 426: return "Upgrade Required";
                    case 428: return "Precondition Required";
                    case 429: return "Too Many Requests";
                    case 431: return "Request Header Fields Too Large";
                    case 451: return "Unavailable For Legal Reasons";

                        // 5xx Server Error
                    case 500: return "Internal Server Error";
                    case 501: return "Not Implemented";
                    case 502: return "Bad Gateway";
                    case 503: return "Service Unavailable";
                    case 504: return "Gateway Timeout";
                    case 505: return "HTTP Version Not Supported";
                    case 506: return "Variant Also Negotiates";
                    case 507: return "Insufficient Storage";
                    case 508: return "Loop Detected";
                    case 510: return "Not Extended";
                    case 511: return "Network Authentication Required";

                    default: return "Unknown Status";
                }
            }


            static bool IsValidStatusCode(const int code) {
                switch (code) {
                    // 1xx
                    case Continue:
                    case SwitchingProtocols:
                    case Processing:
                    case EarlyHints:
                        // 2xx
                    case OK:
                    case Created:
                    case Accepted:
                    case NonAuthoritativeInformation:
                    case NoContent:
                    case ResetContent:
                    case PartialContent:
                    case MultiStatus:
                    case AlreadyReported:
                    case IMUsed:
                        // 3xx
                    case MultipleChoices:
                    case MovedPermanently:
                    case Found:
                    case SeeOther:
                    case NotModified:
                    case UseProxy:
                    case TemporaryRedirect:
                    case PermanentRedirect:
                        // 4xx
                    case BadRequest:
                    case Unauthorized:
                    case PaymentRequired:
                    case Forbidden:
                    case NotFound:
                    case MethodNotAllowed:
                    case NotAcceptable:
                    case ProxyAuthenticationRequired:
                    case RequestTimeout:
                    case Conflict:
                    case Gone:
                    case LengthRequired:
                    case PreconditionFailed:
                    case PayloadTooLarge:
                    case URITooLong:
                    case UnsupportedMediaType:
                    case RangeNotSatisfiable:
                    case ExpectationFailed:
                    case ImATeapot:
                    case MisdirectedRequest:
                    case UnprocessableEntity:
                    case Locked:
                    case FailedDependency:
                    case TooEarly:
                    case UpgradeRequired:
                    case PreconditionRequired:
                    case TooManyRequests:
                    case RequestHeaderFieldsTooLarge:
                    case UnavailableForLegalReasons:
                        // 5xx
                    case InternalServerError:
                    case NotImplemented:
                    case BadGateway:
                    case ServiceUnavailable:
                    case GatewayTimeout:
                    case HTTPVersionNotSupported:
                    case VariantAlsoNegotiates:
                    case InsufficientStorage:
                    case LoopDetected:
                    case NotExtended:
                    case NetworkAuthenticationRequired:
                        return true;
                    default:
                        return false;
                }
            }
        };
    }
}

#endif // HTTPSTATUS_HPP
