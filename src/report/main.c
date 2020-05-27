#include "result.h"

int main(int argc, char *argv[]) {
    //testing named pipes
    int fd;

    string namedPipe="myPipe";
    mkfifo(namedPipe, 0666);
    printf("Errore %s\n", strerror(errno));
    fd=open(namedPipe, O_WRONLY);
    printf("Errore %s\n", strerror(errno));
    string message="ciao cazzon";
    write(fd, message, strlen(message));
    close(fd);
    
    /*int *stats = malloc(CLUSTER*sizeof(int));

    stats[0] = 12;
    stats[1] = 5;
    stats[2] = 3;
    stats[3] = 0;
    stats[4] = 1;

    string *str = statsToString(stats);

    read_result(5, str);*/
    return 0;
}
