#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/constants.h"

typedef struct sockaddr_in sockaddr_in;

char buffer[30000] = {0};

int main(int argc, char * argv[], char * envp[]) {
    printf("Running on port %d\n", PORT);

    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Cannot create socket"); 
        return 0;
    }

    // Bind socket to port
    sockaddr_in * address = (sockaddr_in *)calloc(1, sizeof(sockaddr_in));
    address->sin_family = AF_INET; 
    address->sin_addr.s_addr = htonl(INADDR_ANY); 
    address->sin_port = htons(PORT);
    if (bind(server_fd,(struct sockaddr *)address,sizeof(*address)) < 0) { 
        perror("Bind failed"); 
        return 0;
    }

    if (listen(server_fd, 100) < 0) { 
        perror("Cannot listen for connections"); 
        exit(EXIT_FAILURE); 
    }

    char * hello = "Hello dude"; 
    int addrlen = sizeof(address);

    int k = 0;
    while (1) {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        recv(new_socket, buffer, 30000, 0);
        printf("Receive %s\n", buffer);

        printf("Receive request number #%d\n", ++k);
        close(new_socket);
    }
    

    free(address);
    return 0;
}