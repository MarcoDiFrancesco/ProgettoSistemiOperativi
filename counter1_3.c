#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define MAXLEN 100

typedef char * string;

int N=3;
int M=4;
int CLUSTER=5;

int main(int argc,string argv[]){
    int return_value;
    string toRead=argv[1];
    int fp=open(toRead,O_RDONLY);
    int dim = lseek(fp,0,SEEK_END);
    int part=dim/N;
    //pipes
    int p_c[N][2];
    int q_p[M][2];
    //counters
    int i,j;
}