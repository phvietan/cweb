#include "server.h"

void serverHandle(int socket_fd) {
    char buffer[30000];
    u_int16_t * errorCode = (u_int16_t*)calloc(1, sizeof(u_int16_t));
    
    recv(socket_fd, buffer, sizeof(buffer), 0);

    char * f = getFileContent("./pages/index.html", errorCode);

    char * header = (char *)malloc(80 * sizeof(char));
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n", (int)strlen(f));

    char * response = (char *)malloc((strlen(header) + strlen(f) + 5) * sizeof(char));
    sprintf(response, "%s\r\n%s", header, f);
    send(socket_fd, response, strlen(response), 0);

    printf("Receive %s\n", buffer);
}