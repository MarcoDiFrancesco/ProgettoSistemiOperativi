#include "analyzer.h"

int main() {
    //creo una named pipe con il processo counter per passarsi i dati
    char * myfifo = "myfifo";
    int fd;
    int err=0;
    err=mkfifo(myfifo, 06666);
    string message="ciao";
    fd=open(myfifo, O_WRONLY);
    printf("error %s\n",strerror(errno));
    printf("cacca\n");
    write(fd, message, strlen(message)+1); 
    printf("cacca\n");
    //close(fd);
    string path="./ccopy";
    string arg1="ccopy";
    string arg2="";
    printf("cacca\n");
    execl("/bin/bash", "bash", "-c", "./ccopy", "-r", "testo", NULL);
    printf("cacca\n");
    return 0;
}