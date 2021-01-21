//
// Created by AmazingPP on 2021/1/21.
//

#include "../../include/http/http_request_parser.h"

namespace http {

    HttpRequestParser::HttpCode
    HttpRequestParser::ParseContent(char *buffer, int &check_index, int &read_index,
                                    HttpRequestParser::ParseStates &parse_state, int &start_line,
                                    HttpRequest &request) {
        LineStates line_state = kLineOK;
        HttpCode ret_code = kNoRequest;
        while ((line_state = ParseLine(buffer, check_index, read_index)) == kLineOK) {
            char* temp = buffer + start_line;       // 这一行在buffer中的起始位置
            start_line = check_index;               // 下一行起始位置

            switch (parse_state) {
                case kParseRequestLine:
                    ret_code = ParseRequestLine(temp, parse_state, request);
                    if (ret_code == kBadRequest)
                        return kBadRequest;
                    break;
                case kParseHeader:
                    ret_code = ParseHeaders(temp, parse_state, request);
                    if (ret_code == kBadRequest)
                        return kBadRequest;
                    else if (ret_code == kGetRequest)
                        return kGetRequest;
                    break;
                case kParseBody:
                    ret_code = ParseBody(temp, request);
                    if (ret_code == kGetRequest)
                        return kGetRequest;
                    else
                        return kBadRequest;
                    break;
                default:
                    return kInternalError;
            }
        }

        if (line_state == kLineMore) {
            return kNoRequest;
        } else {
            return kBadRequest;
        }
    }

    HttpRequestParser::LineStates
    HttpRequestParser::ParseLine(char *buffer, int &checked_index, int &read_index) {
        while (checked_index < read_index) {
            char c = buffer[checked_index];
            if (c == kCR) {
                // 到末尾，需要读入更多
                if (checked_index + 1 == read_index)
                    return kLineMore;
                // 完整 "\r\n"
                if (buffer[checked_index + 1] == kLF) {
                    buffer[checked_index++] = kEnd;
                    buffer[checked_index++] = kEnd;
                    return kLineOK;
                }

                return kLineBad;
            }
            ++checked_index;
        }

        // 需要读入更多
        return kLineMore;
    }

    HttpRequestParser::HttpCode
    HttpRequestParser::ParseRequestLine(char *line, HttpRequestParser::ParseStates &parse_state, HttpRequest &request) {
        // method
        char *url = std::strpbrk(line, " \t");
        if (!url)
            return kBadRequest;

        *url++ = '\0';

        char *method = line;
        if (strncasecmp(method, "GET", 3) == 0) {
            request.method = HttpRequest::kGet;
        } else if (strncasecmp(method, "POST", 4) == 0) {
            request.method = HttpRequest::kPost;
        } else if (strncasecmp(method, "PUT", 3) == 0) {
            request.method = HttpRequest::kPut;
        } else {
            return kBadRequest;
        }

        // version
        url += strspn(url, " \t");
        char *version = strpbrk(url, " \t");
        if (!version)
            return kBadRequest;
        *version++ = kEnd;
        version += strspn(version, " \t");

        if (strncasecmp("HTTP/1.1", version, 8) == 0) {
            request.version = HttpRequest::kHttp11;
        } else if (strncasecmp("HTTP/1.0", version, 8) == 0) {
            request.version = HttpRequest::kHttp10;
        } else {
            return kBadRequest;
        }

        // url
        if (strncasecmp(url, "http://", 7) == 0) {
            url +=7;
            url = strchr(url,'/');
        } else if (strncasecmp(url, "/", 1) == 0) {
        } else {
            return kBadRequest;
        }

        if (!url || *url != '/') {
            return kBadRequest;
        }
        request.url = std::string(url);

        parse_state = kParseHeader;
        return kNoRequest;
    }

    HttpRequestParser::HttpCode
    HttpRequestParser::ParseHeaders(const char *line, HttpRequestParser::ParseStates &parse_state, HttpRequest &request) {
        if (*line == kEnd) {
            if (request.method == HttpRequest::kGet) {
                return kGetRequest;
            } else {
                parse_state = kParseBody;
                return kNoRequest;
            }
        }

        auto [key, value] = utils::ParseToKeyValue(line);
        std::transform(key.begin(), key.end(), key.begin(), [](char c) { return std::toupper(c);});

        if (auto it = kHeaderMap.find(utils::Trim(key)); it != kHeaderMap.end()) {
            request.headers.emplace(it->second, utils::Trim(value));
        } else {
            // Warning : not support header
        }

        return kNoRequest;
    }

    HttpRequestParser::HttpCode
    HttpRequestParser::ParseBody(char *body, HttpRequest &request) {
        request.content = body;
        return kGetRequest;
    }
}