//
// Created by AmazingPP on 2021/1/21.
//

#ifndef WEBSERVER_HTTP_DATA_H
#define WEBSERVER_HTTP_DATA_H

#include "../framework.h"
#include "http_request.h"
#include "http_request_parser.h"
#include "http_response.h"
#include "../sockets/client_socket.h"
#include "../timer.h"

namespace timers {
    class Timer;
}
using namespace timers;

namespace http {

    class HttpData : std::enable_shared_from_this<HttpData> {
    public:
        HttpData();

        void CloseTimer();

        void set_timer(std::shared_ptr<Timer> timer);

        int epoll_fd;
        std::shared_ptr<HttpRequest> request;
        std::shared_ptr<HttpResponse> response;
        std::shared_ptr<sockets::ClientSocket> client_socket;
    private:
        std::weak_ptr<Timer> timer_;
    };
}


#endif //WEBSERVER_HTTP_DATA_H
