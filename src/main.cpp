//
// Created by AmazingPP on 2021/1/18.
//

#include "../include/framework.h"
#include "../include/http/http_server.h"

int main(int argc, char** argv) {
    std::string base_path = ".";
    int thread_num = 4, opt;
    uint16_t port = 8000;

    while ((opt = getopt(argc, argv, "t:p:r")) != -1) {
        switch (opt) {
            case 't':
                thread_num = atoi(optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'r':
            {
                if (!utils::IsPathVaild(optarg)) {
                    LOG_ERROR("\"%s\" 不存在或不可访问!", optarg);
                    std::exit(EXIT_FAILURE);
                }
                if (char& end = optarg[std::strlen(optarg) - 1]; end == '/') {
                    end = '\0';
                }
                base_path = optarg;
                break;
            }
            default:
                break;
        }
    }

    logger::Info("*******  Effective 配置信息 *******\n");
    logger::Info("端口号:\t%d\n", port);
    logger::Info("线程数:\t%d\n", thread_num);
    logger::Info("根目录:\t%s\n", base_path.c_str());

    signal(SIGPIPE, SIG_IGN);

    HttpServer server(port);
    server.Run(thread_num);

    return 0;
}
