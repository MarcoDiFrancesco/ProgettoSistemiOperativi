#include "result.h"

int main(int argc, char *argv[]) {
    //testing named pipes
    int fd;
    int rd;
    string namedPipe="./myPipe";
    mkfifo(namedPipe, 0666);
    printf("Errore fifo : %s\n", strerror(errno));
    fd=open(namedPipe, O_RDONLY);
    printf("Errore open : %s\n", strerror(errno));
    string message;
    rd = read(fd, message, 100);
    printf("Errore read : %s\n", strerror(errno));
    close(fd);
    printf("messaggio %s\n", message);


    fd=open(namedPipe, O_RDONLY);
    printf("Errore open : %s\n", strerror(errno));
    rd = read(fd, message, 100);
    printf("Errore read : %s\n", strerror(errno));
    close(fd);
    printf("messaggio %s\n", message);
    
    return 0;
}