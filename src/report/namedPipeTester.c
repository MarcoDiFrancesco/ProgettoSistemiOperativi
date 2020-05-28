#include "result.h"

int main(int argc, char *argv[]) {
    //testing named pipes
    int fd;
    int rd;

    string namedPipe="./myPipe.txt";
    fd=open(namedPipe, O_RDONLY);
    string message;

    //legge 6 caratteri alla volta 
    do{
        rd = read(fd, message, 6);
    }while(rd != 0);

    close(fd);
    printf("messaggio %s\n", message);
    return 0;
}