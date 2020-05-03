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

string readAndWait(int pipe[], pid_t son);
int writePipe(int pipe[],string msg);

int main(int argc,string argv[]){
    int p[2];
    pipe(p);
    int f=fork();
    if(f>0){
        printf("%s",readAndWait(p,f));

    }else{
        writePipe(p,"fra\n");
    }
}


string readAndWait(int pipe[], pid_t son){
    close(pipe[WRITE]);
    string msg;
    msg=malloc(MAXLEN);
    int rd=read(pipe[READ],msg, MAXLEN);
    close(pipe[READ]);
    int err=waitpid(son,NULL,0);
    return msg;
}

int writePipe(int pipe[],string msg){
    int ret=0;//per eventuali errori
    close(pipe[READ]);
    write(pipe[WRITE],msg,strlen(msg)+1);
    close(pipe[WRITE]);
    return ret;
}