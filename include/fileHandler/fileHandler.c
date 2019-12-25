#include "fileHandler.h"

char * getFileContent(char * path, u_int16_t * errorCode) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Cannot open file"); 
        *errorCode = 404;
    }
    else {
        struct stat st;
        stat(path, &st);
        int size = st.st_size;

        char * result = (char *)malloc(size * sizeof(char));
        read(fd, result, size);
        return result;
    }
}