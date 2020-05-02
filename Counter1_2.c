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

    for(j=0;j<N;j++){
        pipe(p_c[j]);
        int pson=fork();
        if(pson<0){
            printf("Errore di creazione figlio\n");
            exit(2);
        }
        if(pson>0){
            //processo C
            printf("C created\n");
            close(p_c[j][WRITE]);
            char msgPC[MAXLEN];
            int rd=read(q_p[i][READ],msgPC, MAXLEN);
            //printf("mess%d=%s\n",i,msgPC);
            close(p_c[j][READ]);
            wait(NULL);
            printf("Q=%d p=%d\n",q,p);
        }else{
            //PROCESSO P
            //printf("P created\n");
            p++;
            for(i=0;i<M;i++){
                pipe(q_p[i]);
                int qson=fork();
                if(qson<0){
                    printf("Errore di creazione figlio\n");
                    exit(2);
                }
                if(qson>0){
                    //dialogo con Q
                    close(q_p[i][WRITE]);
                    char msgQP[MAXLEN];
                    int rd=read(q_p[i][READ],msgQP, MAXLEN);
                    //printf("mess%d=%s\n",i,msgQP);
                    close(q_p[i][READ]);
                    wait(NULL);
                    //diaologo con C
                    close(p_c[j][READ]);
                    string msgPC="La puttna della zia";
                    write(p_c[j][WRITE],msgPC,strlen(msgPC)+1);
                    close(p_c[j][WRITE]);
                }else{
                    //PROCESSO Q
                    //printf("Q created\n");
                    q++;
                    close(q_p[i][READ]);
                    string msgQP="La puttna della mamma";
                    write(q_p[i][WRITE],msgQP,strlen(msgQP)+1);
                    close(q_p[i][WRITE]);
                }
            }
        }
    }

    
    return 0;
}