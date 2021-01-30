//
// Created by AmazingPP on 2021/1/20.
//

#include "../../include/http/http_response.h"

namespace http {
    HttpResponse::HttpResponse(bool keep_alive) :
        status_code_(kUnknown),
        keep_alive_(keep_alive),
        mime_("text/html"),
        body_(nullptr),
        version_(HttpRequest::kHttp11) {}

    HttpResponse::~HttpResponse() {
        if (body_)
            delete[] body_;
    }

    void HttpResponse::AppendBuffer(fmt::memory_buffer &buffer) const {
        // 构造状态行
        fmt::format_to(buffer,
                       "HTTP/1.{} {} {}\r\n",
                       version_ == HttpRequest::kHttp10 ? 0 : 1,
                       status_code_,
                       status_msg_);

        // 构造消息报头
        for (auto [key, value] : headers_) {
            fmt::format_to(buffer, "{}: {}\r\n", key, value);
        }
        fmt::format_to(buffer, "Content-type: {}\r\n", mime_.type);
        fmt::format_to(buffer, "Connection: {}\r\n", keep_alive_ ? "keep-alive" : "close");

//        if (version_ == HttpRequest::HttpVersions::kHttp10) {
//            sprintf(buffer, "HTTP/1.1 %d %s\r\n", status_code_, status_msg_.c_str());
//        } else {
//            sprintf(buffer, "HTTP/1.0 %d %s\r\n", status_code_, status_msg_.c_str());
//        }
//
//        for (auto [key, value] : headers_) {
//            sprintf(buffer, "%s%s: %s\r\n", buffer, key.c_str(), value.c_str());
//        }
//        sprintf(buffer, "%sContent-type: %s\r\n", buffer, mime_.type.c_str());
//
//        if (keep_alive_) {
//            sprintf(buffer, "%sConnection: keep-alive\r\n", buffer);
//        } else {
//            sprintf(buffer, "%sConnection: close\r\n", buffer);
//        }
    }

    void HttpResponse::AddHeader(const std::string &key, const std::string &value) {
        headers_[key] = value;
    }

    HttpResponse::HttpStatusCode HttpResponse::status_code() const {
        return status_code_;
    }

    void HttpResponse::set_status_code(HttpResponse::HttpStatusCode statusCode) {
        status_code_ = statusCode;
    }

    const std::string &HttpResponse::status_msg() const {
        return status_msg_;
    }

    void HttpResponse::set_status_msg(const std::string &statusMsg) {
        status_msg_ = statusMsg;
    }

    HttpRequest::HttpVersions HttpResponse::version() const {
        return version_;
    }

    void HttpResponse::set_version(HttpRequest::HttpVersions version) {
        version_ = version;
    }

    bool HttpResponse::keep_alive() const {
        return keep_alive_;
    }

    void HttpResponse::set_keep_alive(bool keepAlive) {
        keep_alive_ = keepAlive;
    }

    const MimeType &HttpResponse::mime() const {
        return mime_;
    }

    void HttpResponse::set_mime(const MimeType &mime) {
        mime_ = mime;
    }

    const char *HttpResponse::body() const {
        return body_;
    }

    void HttpResponse::set_body(const char *body) {
        body_ = body;
    }

    int HttpResponse::content_length() const {
        return content_length_;
    }

    void HttpResponse::set_content_length(int contentLength) {
        content_length_ = contentLength;
    }

    const std::string &HttpResponse::file_path() const {
        return file_path_;
    }

    void HttpResponse::set_file_path(const std::string &filePath) {
        file_path_ = filePath;
    }
}