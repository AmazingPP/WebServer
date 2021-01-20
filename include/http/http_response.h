//
// Created by AmazingPP on 2021/1/20.
//

#pragma once

#ifndef WEBSERVER_HTTP_RESPONSE_H
#define WEBSERVER_HTTP_RESPONSE_H

#include "../framework.h"
#include "http_request.h"
#include "mime_type.h"

namespace http {
    class HttpResponse {

        enum HttpStatusCode {
            kUnknown,
            kOk = 200,
            kForbidden = 403,
            kNotFound = 404
        };

    public:
        explicit HttpResponse(bool keep_alive = true);

        ~HttpResponse();

        void AppendBuffer(char* const buffer);

        void AddHeader(const std::string &key, const std::string &value);

        HttpStatusCode getStatusCode() const;

        void setStatusCode(HttpStatusCode statusCode);

        const std::string &getStatusMsg() const;

        void setStatusMsg(const std::string &statusMsg);

        HttpRequest::HttpVersions getVersion() const;

        void setVersion(HttpRequest::HttpVersions version);

        bool isKeepAlive() const;

        void set_keep_alive(bool keepAlive);

        const MimeType &mime() const;

        void set_mime(const MimeType &mime);

        const char *body() const;

        void set_body(const char *body);

        int content_length() const;

        void set_content_length(int contentLength);

        const std::string &file_path() const;

        void set_file_path(const std::string &filePath);

    private:
        HttpStatusCode status_code_;
        std::string status_msg_;
        HttpRequest::HttpVersions version_;
        bool keep_alive_;
        MimeType mime_;
        const char* body_;
        int content_length_;
        std::string file_path_;
        std::unordered_map<std::string, std::string> headers_;
    };
}


#endif //WEBSERVER_HTTP_RESPONSE_H
