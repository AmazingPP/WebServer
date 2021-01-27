//
// Created by AmazingPP on 2021/1/21.
//

#ifndef WEBSERVER_HTTP_REQUEST_PARSER_H
#define WEBSERVER_HTTP_REQUEST_PARSER_H

#include "../framework.h"
#include "http_request.h"
namespace http {
    class HttpRequestParser {
    public:
        constexpr static char kCR = '\r';
        constexpr static char kLF = '\n';
        constexpr static char kEnd = '\0';

        enum LineStates {
            kLineOK,
            kLineBad,
            kLineMore
        };

        enum ParseStates {
            kParseRequestLine,
            kParseHeader,
            kParseBody
        };

        enum HttpCode {
            kNoRequest,
            kGetRequest,
            kBadRequest,
            kForbiddenRequest,
            kInternalError,
            kClosedConnection
        };

        // 解析Http请求
        static HttpCode ParseContent(char *buffer, int &check_index, int &read_index,
                                     ParseStates &parse_state, int &start_line,
                                     HttpRequest &request);

    private:
        // 解析一行内容, buffer[checked_index, read_index)
        // check_index是需要分析的第一个字符， read_index已经读取数据末尾下一个字符
        static LineStates ParseLine(char *buffer, int &checked_index, int &read_index);
        
        static HttpCode ParseRequestLine(char *line, ParseStates &parse_state, HttpRequest &request);

        static HttpCode ParseHeaders(const char *line, ParseStates &parse_state, HttpRequest &request);

        static HttpCode ParseBody(char *body, HttpRequest &request);
    };
}

#endif //WEBSERVER_HTTP_REQUEST_PARSER_H
