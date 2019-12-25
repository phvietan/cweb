#include "server.h"

int isUpload(http_req * req) {
    if (strncmp(req->method, "POST", 4) != 0) return 0;
    return req->contentLength > 0;
}

char * requestReceive(const int socket_fd) {
    // receive up to 65kb request only
    const int maxSize = (1 << 16) * sizeof(char);
    char * req = (char *)malloc(maxSize);
    int len = recv(socket_fd, req, maxSize, 0);
    return (char*)realloc(req, len);
}

void responseHandler(const int socket_fd, http_req * req) {
    char * response;

    // Max size is 1mb
    if (req->contentLength > (1<<20)) {
        response = "HTTP/1.1 413 Payload Too Large\r\n";
    }
    else if (isUpload(req)) {
        puts("Receiving file");
        int bytesReceived = 0;
        int processedHeader = 0;
        
        while (bytesReceived < req->contentLength) {
            // Send more pl0z
            response = "HTTP/1.1 100 Continue\r\n";
            send(socket_fd, response, strlen(response), 0);


            // Receive
            char * buf = (char *)malloc((1<<16) * sizeof(char));
            int len = recv(socket_fd, buf, 1<<16, 0);
            printf("%d\n", len);

            FILE * fp = fopen("myfile.txt", "ab");
            fwrite(buf, len, 1, fp);

            bytesReceived += len;
            free(buf);
        }
        response = "HTTP/1.1 201 Created\r\n";
    }
    else if (strncmp(req->path, "/", 1) == 0) {
        u_int16_t * errorCode = (u_int16_t*)calloc(1, sizeof(u_int16_t));
        char * f = getFileContent(INDEX_DIR, errorCode);
        char * header = (char *)malloc(80 * sizeof(char));
        sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n", (int)strlen(f));

        response = (char *)malloc((strlen(header) + strlen(f) + 5) * sizeof(char));
        sprintf(response, "%s\r\n%s", header, f);
    } else {
        response = "HTTP/1.1 404 Not Found\r\n";
    }
    send(socket_fd, response, strlen(response), 0);
}

void serverHandle(const int socket_fd) {
    char * buf = requestReceive(socket_fd);
    http_req * req = parseRequest(buf);
    responseHandler(socket_fd, req);
    // printf("%s\n", buf);
    free(req);
    free(buf);
}