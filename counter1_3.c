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
    printf("Process C pid=%d\n",getpid());

    for(i=0;i<N;i++){
        //creo N processi di tipo P
        int c_son=fork();
        if(c_son==-1){
            printf("error occurred at line 35\n");
            return_value=35;
        }else{
            if(c_son==0){
                //processo P
                printf("P created pid=%d ppid=%d\n",getpid(),getppid());

                //creo M processi di tipo Q
                for(j=0;j<M;j++){
                    int p_son=fork();
                    if(p_son==-1){
                        printf("error occurred at line 46\n");
                        return_value=46;
                    }else{
                        if(p_son==0){
                            //processo Q
                            printf("\tQ created pid=%d ppid=%d\n",getpid(),getppid());

                            //logica processo  q
                            exit(0);
                        }else{
                            //successive parti del processo P
                            waitpid(p_son,NULL,0);
                        }
                        
                    }
                }
                //potenziale uscita del processo P
                exit(0);
            }else{
                //successive parti del processo C
                waitpid(c_son,NULL,0);
            }
        }
    }
    return return_value;
}