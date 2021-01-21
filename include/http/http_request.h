//
// Created by AmazingPP on 2021/1/20.
//

#pragma once

#ifndef WEBSERVER_HTTP_REQUEST_H
#define WEBSERVER_HTTP_REQUEST_H

#include "../framework.h"

namespace http {
    struct HttpRequest {

        enum HttpVersions {
            kHttp10,
            kHttp11,
            kVersionNotSupport
        };

        enum HttpMethods {
            kGet,
            kPost,
            kPut,
            kDelete,
            kMethodNotSupport
        };

        enum HttpHeaders {
            kHost,
            kUserAgent,
            kConnection,
            kAccept,
            kAcceptEncoding,
            kAcceptLanguage,
            kCacheControl,
            kUpgradeInsecureRequests
        };

        struct EnumClassHash {
            template<typename T>
            std::size_t operator()(T t) const {
                return static_cast<std::size_t>(t);
            }
        };

        HttpRequest(std::string url = std::string(""),
                    HttpMethods method = HttpMethods::kMethodNotSupport,
                    HttpVersions version = HttpVersions::kVersionNotSupport) :
                    method(method), version(version), url(url), content(nullptr) {}

        friend std::ostream &operator<<(std::ostream &os, const HttpRequest &request);

        HttpMethods method;
        HttpVersions version;
        std::string url;
        char* content;
        std::unordered_map<HttpHeaders, std::string, EnumClassHash> headers;
    };

    const static std::unordered_map<std::string, HttpRequest::HttpHeaders> kHeaderMap {
            {"HOST",                      HttpRequest::HttpHeaders::kHost},
            {"USER-AGENT",                HttpRequest::HttpHeaders::kUserAgent},
            {"CONNECTION",                HttpRequest::HttpHeaders::kConnection},
            {"ACCEPT-ENCODING",           HttpRequest::HttpHeaders::kAcceptEncoding},
            {"ACCEPT-LANGUAGE",           HttpRequest::HttpHeaders::kAcceptLanguage},
            {"ACCEPT",                    HttpRequest::HttpHeaders::kAccept},
            {"CACHE-CONTROL",             HttpRequest::HttpHeaders::kCacheControl},
            {"UPGRADE-INSECURE-REQUESTS", HttpRequest::HttpHeaders::kUpgradeInsecureRequests}
    };
}

#endif //WEBSERVER_HTTP_REQUEST_H
