/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:35:16 by yroard            #+#    #+#             */
/*   Updated: 2026/01/21 15:42:59 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

namespace webserv {
	typedef enum eVerb {
		GET, POST, DELETE
	}tVerb;

    typedef enum eStoreStatus {
        OFF = 0,
		ON = 1,
		ERROR = -1
    }tStoreStatus;

}

        typedef enum eStatusCode{
            // 1xx Informational
            Continue = 100,
            SwitchingProtocols = 101,
            Processing = 102, // WebDAV
            EarlyHints = 103,

            // 2xx Success
            OK = 200,
            Created = 201,
            Accepted = 202,
            NonAuthoritativeInformation = 203,
            NoContent = 204,
            ResetContent = 205,
            PartialContent = 206,
            MultiStatus = 207, // WebDAV
            AlreadyReported = 208, // WebDAV
            IMUsed = 226,

            // 3xx Redirection
            MultipleChoices = 300,
            MovedPermanently = 301,
            Found = 302,
            SeeOther = 303,
            NotModified = 304,
            UseProxy = 305,
            TemporaryRedirect = 307,
            PermanentRedirect = 308,

            // 4xx Client Error
            BadRequest = 400,
            Unauthorized = 401,
            PaymentRequired = 402,
            Forbidden = 403,
            NotFound = 404,
            MethodNotAllowed = 405,
            NotAcceptable = 406,
            ProxyAuthenticationRequired = 407,
            RequestTimeout = 408,
            Conflict = 409,
            Gone = 410,
            LengthRequired = 411,
            PreconditionFailed = 412,
            PayloadTooLarge = 413,
            URITooLong = 414,
            UnsupportedMediaType = 415,
            RangeNotSatisfiable = 416,
            ExpectationFailed = 417,
            ImATeapot = 418, // Easter egg (RFC 2324)
            MisdirectedRequest = 421,
            UnprocessableEntity = 422, // WebDAV
            Locked = 423, // WebDAV
            FailedDependency = 424, // WebDAV
            TooEarly = 425,
            UpgradeRequired = 426,
            PreconditionRequired = 428,
            TooManyRequests = 429,
            RequestHeaderFieldsTooLarge = 431,
            UnavailableForLegalReasons = 451,

            // 5xx Server Error
            InternalServerError = 500,
            NotImplemented = 501,
            BadGateway = 502,
            ServiceUnavailable = 503,
            GatewayTimeout = 504,
            HTTPVersionNotSupported = 505,
            VariantAlsoNegotiates = 506,
            InsufficientStorage = 507, // WebDAV
            LoopDetected = 508, // WebDAV
            NotExtended = 510,
            NetworkAuthenticationRequired = 511
        }tStatusCode;

#endif //WEBSERV_WEBSERV_HPP