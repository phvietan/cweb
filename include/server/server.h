#ifndef SERVER_H
#define SERVER_H 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "../httpParser/httpParser.h"
#include "../fileHandler/fileHandler.h"

#include "../constants.h"

void serverHandle(const int socket_fd);

#endif