#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


#define READ 0
#define WRITE 1
#define MAXLEN 100
#define BOOL char
#define FALSE 0
#define TRUE 1
#define BUFFSIZE 1000

typedef char * string;

int writePipe(int pipe[],string msg);
string readAndWait(int pipe[], pid_t son);

//funzioni di ric

int readFile(char* filename, char* filedata, int start, int stop);

void countLetters(int dim, char* s, int* counter);

int* processoQ(int from, int to, char* fname);

int* processoQ_n(int from, int to, char** fname, int n);