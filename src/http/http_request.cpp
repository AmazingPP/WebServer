//
// Created by AmazingPP on 2021/1/20.
//

#include "../../include/http/http_request.h"

namespace http {
    std::ostream &operator<<(std::ostream &os, const HttpRequest &request) {
        os << "method: " << request.method << std::endl;
        os << "version: " << request.version << std::endl;
        os << "url: " << request.url << std::endl;
        for(auto& [key, value] : request.headers) {
            os << key << ": " << value << std::endl;
        }
        return os;
    }
}