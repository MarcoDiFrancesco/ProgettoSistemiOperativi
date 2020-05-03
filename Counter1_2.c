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

int N=2;
int M=2;
int CLUSTER=5;
//test
//debug
int p=0;
int q=0;
//debug

int main(int argc,string argv[]){
    string file=argv[1];
    int fp=open(argv[1],O_RDONLY);
    int dim = lseek(fp,0,SEEK_END);
    int part=dim/N;
    int p_c[N][2];
    int q_p[M][2];
    //creo N processi figli con cui stabilisco un pipe
    int i;
    int j;
    printf("C created pid=%d\n",getpid());
    for(j=0;j<N;j++){
        pipe(p_c[j]);
        pid_t pson=fork();
        if(pson<0){
            printf("Errore di creazione figlio\n");
            exit(2);
        }
        if(pson>0){
            //processo C
            close(p_c[j][WRITE]);
            char msgPC[MAXLEN];
            int rd=read(q_p[i][READ],msgPC, MAXLEN);
            //printf("mess%d=%s\n",i,msgPC);
            close(p_c[j][READ]);
            wait(NULL);
            printf("aaa");
            //printf("Q=%d p=%d\n",q,p);
        }else{
            //PROCESSO P
            printf("\t P created pid=%d  ppid=%d\n",getpid(),getppid());
            for(i=0;i<M;i++){
                //printf("Cacca\n");
                pipe(q_p[i]);
                pid_t qson=fork();
                if(qson<0){
                    printf("Errore di creazione figlio\n");
                    exit(202);
                }
                if(qson>0){
                    //P parla con Q
                    close(q_p[i][WRITE]);
                    char msgQP[MAXLEN];
                    int rd=read(q_p[i][READ],msgQP, MAXLEN);
                    //printf("mess%d=%s\n",i,msgQP);
                    close(q_p[i][READ]);
                    //waitpid(qson,NULL,NULL);
                    //diaologo con C
                    close(p_c[j][READ]);
                    string msgPC="La puttna della zia";
                    write(p_c[j][WRITE],msgPC,strlen(msgPC)+1);
                    close(p_c[j][WRITE]);
                }else{
                    //PROCESSO Q
                    printf("Q created ppid=%d\n",getppid());
                    close(q_p[i][READ]);
                    string msgQP="La puttna della mamma";
                    write(q_p[i][WRITE],msgQP,strlen(msgQP)+1);
                    close(q_p[i][WRITE]);
                    exit(1);
                }
            }
        }
        if(pson==0){
            printf("\tp closed \n");
            exit(33);
        }        
    }

    return 0;
}