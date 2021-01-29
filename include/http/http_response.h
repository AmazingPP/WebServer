//
// Created by AmazingPP on 2021/1/20.
//

#ifndef WEBSERVER_HTTP_RESPONSE_H
#define WEBSERVER_HTTP_RESPONSE_H

#include "../framework.h"
#include "http_request.h"
#include "mime_type.h"

namespace http {
    class HttpResponse {
    public:
        enum HttpStatusCode {
            kUnknown,
            kOk = 200,
            kForbidden = 403,
            kNotFound = 404
        };

        explicit HttpResponse(bool keep_alive = true);

        ~HttpResponse();

        void AppendBuffer(char* buffer) const;

        void AddHeader(const std::string &key, const std::string &value);

        HttpStatusCode status_code() const;

        void set_status_code(HttpStatusCode status_code);

        const std::string &status_msg() const;

        void set_status_msg(const std::string &status_msg);

        HttpRequest::HttpVersions version() const;

        void set_version(HttpRequest::HttpVersions version);

        bool keep_alive() const;

        void set_keep_alive(bool keep_alive);

        const MimeType &mime() const;

        void set_mime(const MimeType &mime);

        const char *body() const;

        void set_body(const char *body);

        int content_length() const;

        void set_content_length(int content_length);

        const std::string &file_path() const;

        void set_file_path(const std::string &file_path);

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
