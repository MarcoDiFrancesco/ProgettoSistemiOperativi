#include "result.h"

int main(int argc, char *argv[]) {
    //testing named pipes
    int fd;
    string namedPipe="./myPipe";
    fd=open(namedPipe, O_RDONLY);
    string message;
    read(fd, message, 100);
    close(fd);
    unlink(namedPipe);
    printf("messaggio %s\n", message);
    return 0;
}