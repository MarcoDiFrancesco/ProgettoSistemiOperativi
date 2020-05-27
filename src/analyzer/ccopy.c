#include "analyzer.h"

int main(int argc, string argv[]) {
    printf("ccopy\n");
    char * myfifo = "myfifo";
    int fd;
    string message;
    fd = open(myfifo, O_RDONLY); 
    read(fd, message, 80); 
    printf("messaggio: %s\n",message);
    close(fd);
}