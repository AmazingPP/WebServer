//
// Created by AmazingPP on 2021/1/18.
//

#include "../include/framework.h"
#include "../include/utils.h"

int main(int argc, char** argv) {
    std::string base_path = ".";
    int thread_num = 4, port = 8080;

    while (int opt = getopt(argc, argv, "t:p:r") != -1) {
        switch (opt) {
            case 't':
                thread_num = atoi(optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'r':
            {
                if (!utils::check_path_vail(optarg)) {
                    printf("Warning: \"%s\" 不存在或不可访问! \n", optarg);
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

    printf("\"******* WebServer Config Info *******\\n");
    printf("Port:\t%d\n",port);
    printf("Thread number:\t%d\n", thread_num);
    printf("Base path:\t%s\n",base_path.c_str());

    utils::handle_for_sigpipe();

    return 0;
}
