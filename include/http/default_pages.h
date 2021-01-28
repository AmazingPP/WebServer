//
// Created by AmazingPP on 2021/1/28.
//

#ifndef WEBSERVER_DEFAULT_PAGES_H
#define WEBSERVER_DEFAULT_PAGES_H

namespace http {
    constexpr char kNotFoundPage[] = "<html>\n"
                                     "<head><title>404 Not Found</title></head>\n"
                                     "<body style=\"background-color: white\">\n"
                                     "<center><h1>404 Not Found</h1></center>\n"
                                     "<hr><center> Effective/0.1 (Linux) </center>\n"
                                     "</body>\n"
                                     "</html>";

    constexpr char kForbiddenPage[] = "<html>\n"
                                      "<head><title>403 Forbidden</title></head>\n"
                                      "<body style=\"background-color: white\">\n"
                                      "<center><h1>403 Forbidden</h1></center>\n"
                                      "<hr><center> Effective/0.1 (Linux) </center>\n"
                                      "</body>\n"
                                      "</html>";

    constexpr char kIndexPage[] = "<!DOCTYPE html>\n"
                                  "<html>\n"
                                  "<head>\n"
                                  "    <title>Welcome to Effective!</title>\n"
                                  "    <style>\n"
                                  "        body {\n"
                                  "            width: 35em;\n"
                                  "            margin: 0 auto;\n"
                                  "            font-family: Tahoma, Verdana, Arial, sans-serif;\n"
                                  "        }\n"
                                  "    </style>\n"
                                  "</head>\n"
                                  "<body>\n"
                                  "<h1>Welcome to Effective!</h1>\n"
                                  "<p>If you see this page, the Effective is successfully installed and\n"
                                  "    working. </p>\n"
                                  "\n"
                                  "<p>For online documentation and support please refer to\n"
                                  "    <a href=\"https://github.com/AmazingPP/WebServer\"> Effective </a>.<br/>\n"
                                  "\n"
                                  "<p><em>Thank you for using Effective.</em></p>\n"
                                  "</body>\n"
                                  "</html>";
}

#endif //WEBSERVER_DEFAULT_PAGES_H
