#include "httpParser.h"

// Beware of upload file attack '\x00'
int shouldStop(char c) {
    return c == '\n' || c == '\r' || c == '\x00';
}

char * getMethod(char * buf) {
    // method length is up to 10 bytes
    char * method = malloc(10 * sizeof(char));
    int curLen = 0;
    for (int i = 0; i < 10; ++i) {
        if (shouldStop(buf[i]) || buf[i] == ' ') {
            method[i] = '\x00';
            break;
        }
        else method[curLen++] = buf[i];
    }
    return (char *)realloc(method, curLen * sizeof(char));
}

char * getPath(char * buf) {
    // path length is up to 8kb
    char * path = (char *)malloc((1<<13) * sizeof(char));
    int curLen = 0;
    int spaceCount = 0;
    for (char * it = buf; spaceCount < 2; ++it) {
        char cur = *it;
        if (shouldStop(cur)) break;
        if (cur == ' ') {
            spaceCount++;
            if (spaceCount == 2) path[curLen++] = '\x00';
            continue;
        }
        if (spaceCount == 0) continue;
        path[curLen++] = cur;
    }
    return (char *)realloc(path, curLen * sizeof(char));
}

char * getVersion(char * buf) {
    // path length is up to 10 bytes
    char * version = (char *)malloc(10 * sizeof(char));
    int curLen = 0;
    int spaceCount = 0;
    for (char * it = buf;; ++it) {
        char cur = *it;
        if (shouldStop(cur)) {
            version[curLen++] = '\x00';
            break;
        }
        if (cur == ' ') {
            spaceCount++;
            continue;
        }
        if (spaceCount == 2) version[curLen++] = cur;
    }
    return (char *)realloc(version, curLen * sizeof(char));
}

int findNextSpace(char * buf, int i) {
    while (buf[i] != ' ' && buf[i] != '\x00') ++i;
    if (buf[i] == ' ') return i;
    return -1;
}

void getAttributes(char * buf, http_req * req) {
    const int attributesSize = 6;
    char * attributes[6] = {"Host", "Connection", "Referer", "Cookie", "Content-Type", "Content-Length"};

    char * host = (char *)malloc(256 * sizeof(char));
    char * referer = (char *)malloc(256 * sizeof(char));
    char * cookie = (char *)malloc(4096 * sizeof(char));
    char * connection = (char *)malloc(16 * sizeof(char));
    char * contentType = (char *)malloc(256 * sizeof(char));
    char * contentLength = (char *)malloc(12 * sizeof(char));

    char * pointers[6] = {host, connection, referer, cookie, contentType, contentLength};

    const int n = (int)strlen(buf);
    for (int i = 1; i < n; ++i) {
        if (buf[i-1] != '\r' && buf[i-1] != '\n') continue;
        for (int j = 0; j < attributesSize; ++j)
            if (strncmp(buf+i, attributes[j], strlen(attributes[j])) == 0) {
                int space = findNextSpace(buf, i);
                int curLen = 0;
                for (int k = space+1; !shouldStop(buf[k]); ++k)
                    pointers[j][curLen++] = buf[k];
                pointers[j][curLen++] = '\x00';
                pointers[j] = (char*)realloc(pointers[j], curLen*sizeof(char));
            }
    }
    req->host = pointers[0];
    req->connection = pointers[1];
    req->referer = pointers[2];
    req->cookie = pointers[3];
    req->contentType = pointers[4];
    req->contentLength = 0;
    for (int i = 0; i < (int)strlen(pointers[5]); ++i) {
        req->contentLength = req->contentLength*10 + pointers[5][i] - '0';
    }
}

void debug(http_req * req) {
    printf("Method is: %s\n", req->method);
    printf("Path is: %s\n", req->path);
    printf("Version is: %s\n", req->version);
    printf("Host is: %s\n", req->host);
    printf("Connection is: %s\n", req->connection);
    printf("Referer is: %s\n", req->referer);
    printf("Cookie is: %s\n", req->cookie);
    printf("Content-Type is: %s\n", req->contentType);
    printf("Content-Length is: %d\n", req->contentLength);
}

http_req * parseRequest(char * buf) {
    http_req * req = (http_req *)malloc(sizeof(http_req));
    req->path = getPath(buf);
    req->method = getMethod(buf);
    req->version = getVersion(buf);
    getAttributes(buf, req);

    // debug(req);

    return req;
}

void response() {
    puts("none");
}