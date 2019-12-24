#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/server/server.h"
#include "../include/constants.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int getPort() { return atoi(PORT); }

int createSocket() {
    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Cannot create socket"); 
        exit(EXIT_FAILURE); 
    }
    return server_fd;
}

sockaddr_in * createAddress() {
    sockaddr_in * address = (sockaddr_in *)calloc(1, sizeof(sockaddr_in));
    address->sin_family = AF_INET; 
    address->sin_port = htons(getPort());
    address->sin_addr.s_addr = htonl(INADDR_ANY); 
    return address;
}


sockaddr_in * bindSocket(const int socket_fd) {
    sockaddr_in * address = createAddress();
    if (bind(socket_fd,(sockaddr *)address,sizeof(sockaddr_in)) < 0) { 
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, 500) < 0) { 
        perror("Cannot listen for connections"); 
        exit(EXIT_FAILURE);
    }

    return address;
}

void serveRestAPI(const int socket_fd, sockaddr_in * address) {
    int addrlen = sizeof(address);
    while (1) {
        int new_socket = accept(socket_fd, (sockaddr *)address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        static unsigned int reqNum = 0;
        printf("Receive request number #%d\n", ++reqNum);
        int pid = fork();
        if (pid == 0) {
            serverHandle(new_socket);
            close(new_socket);
            break;
        }
        close(new_socket);
    }
    
}

int main(int argc, char * argv[], char * envp[]) {
    printf("Running on port %d\n", getPort());

    const int socket_fd = createSocket();
    sockaddr_in * address = bindSocket(socket_fd);
    serveRestAPI(socket_fd, address);

    return 0;
}