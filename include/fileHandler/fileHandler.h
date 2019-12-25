#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H 1

#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

char * getFileContent(char * path, u_int16_t * errorCode);

#endif