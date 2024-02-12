/**
 * @file    http_status_codes.hpp
 * @brief   This file contains the declaration of the HTTP status codes used in the project.
 * @author  Mert Ozer
 * @date    26.11.2023
 * @version 1.0
 */

#ifndef HTTP_STATUS_CODES_HPP
#define HTTP_STATUS_CODES_HPP


namespace HttpStatus {
    // Success
    constexpr int OK = 200;
    constexpr int CREATED = 201;
    constexpr int ACCEPTED = 202;
    constexpr int NO_CONTENT = 204;

    // Redirection
    constexpr int MOVED_PERMANENTLY = 301;
    constexpr int FOUND = 302;
    constexpr int SEE_OTHER = 303;
    constexpr int NOT_MODIFIED = 304;
    constexpr int TEMPORARY_REDIRECT = 307;
    constexpr int PERMANENT_REDIRECT = 308;

    // Client Errors
    constexpr int BAD_REQUEST = 400;
    constexpr int UNAUTHORIZED = 401;
    constexpr int FORBIDDEN = 403;
    constexpr int NOT_FOUND = 404;
    constexpr int METHOD_NOT_ALLOWED = 405;
    constexpr int NOT_ACCEPTABLE = 406;
    constexpr int CONFLICT = 409;

    // ServerManager Errors
    constexpr int INTERNAL_SERVER_ERROR = 500;
    constexpr int NOT_IMPLEMENTED = 501;
    constexpr int BAD_GATEWAY = 502;
    constexpr int SERVICE_UNAVAILABLE = 503;
    constexpr int GATEWAY_TIMEOUT = 504;
    constexpr int HTTP_VERSION_NOT_SUPPORTED = 505;
}


#endif // HTTP_STATUS_CODES_HPP
