#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "../stringUtils.h"

struct http_req {
    char * path;
    char * host;
    char * cookie;
    char * method;
    char * version;
    char * referer;
    char * connection;
    char * contentType;
    int contentLength;
};

typedef struct http_req http_req;

http_req * parseRequest(char * buf);

#endif