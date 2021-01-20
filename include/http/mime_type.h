//
// Created by AmazingPP on 2021/1/20.
//

#pragma once

#ifndef WEBSERVER_MIME_TYPE_H
#define WEBSERVER_MIME_TYPE_H

#include "../framework.h"

namespace http {
    struct MimeType {

        MimeType(const std::string& str) : type(str) {}

        MimeType(const char* str) : type(str) {}

        std::string type;
    };

    const static std::unordered_map<std::string, MimeType> kMimeMap {
            {".html", "text/html"},
            {".xml", "text/xml"},
            {".xhtml", "application/xhtml+xml"},
            {".txt", "text/plain"},
            {".rtf", "application/rtf"},
            {".pdf", "application/pdf"},
            {".word", "application/msword"},
            {".png", "image/png"},
            {".gif", "image/gif"},
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".au", "audio/basic"},
            {".mpeg", "video/mpeg"},
            {".mpg", "video/mpeg"},
            {".avi", "video/x-msvideo"},
            {".gz", "application/x-gzip"},
            {".tar", "application/x-tar"},
            {".css", "text/css"},
            {"", "text/plain"},
            {"default","text/plain"}
    };
}

#endif //WEBSERVER_MIME_TYPE_H
