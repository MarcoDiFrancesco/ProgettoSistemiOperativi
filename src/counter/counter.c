#ifndef _COUNTER_H_
#define _COUNTER_H_

#include "counter.h"


string *readAndWait(int pipe[], pid_t son){
    close(pipe[WRITE]);
    string *msg;
    msg=malloc(CLUSTER*sizeof(char*));
    int i;
    int rd;
    int err=0;
    for(i=0;i<CLUSTER;i++){
        msg[i]=malloc(MAXLEN);
        rd=read(pipe[READ],msg[i], MAXLEN);
        if(rd==-1){
            err=rd;
        }
        msg[i][rd]=0;
    }
    //printf("(read)ERR=%d",err);
    close(pipe[READ]);
    waitpid(son,NULL,0);
    return msg;
}

int writePipe(int pipe[],string *msg){
    int ret=0;//per eventuali errori
    //close(pipe[READ]);
    int i,err;
    
    for(i=0;i<CLUSTER;i++){
        err=write(pipe[WRITE],msg[i],MAXLEN);
        if(err!=0){
            ret=err;
        }
    }
    close(pipe[WRITE]);
    return ret;
}

int * filesDim(string *files,int num,int M){
    int i;
    string toRead;
    int fp,dim;
    int *ret;
    ret=malloc(num*sizeof(int));
    for(i=0;i<num;i++){
        toRead=files[i];
        fp=open(toRead,O_RDONLY);
        dim = lseek(fp,0,SEEK_END);
        ret[i]=dim/M;
    }
    return ret;
}

//---------------------------ric functions------------------------------------

int readFile(char* filename, char* filedata, int start, int stop){
    int sk, rd;
    int file = open(filename, O_RDONLY);
    if(file>=0){
        sk = lseek(file, start*sizeof(char), SEEK_SET);
        if(sk != -1){
            rd = read(file, filedata, stop-start);
        }
    }
    if(file>=0 && sk>=0 && rd>=0) return 0;
    else return -1;
}

void countLetters(int dim, char* s, int* counter){
    //printf("testo cont: ");
    int i = dim-1;
    for(i; i>=0; i--){
        
        //printf("%c\t", s[i]);
        if(s[i]>='a' && s[i]<='z' || s[i]>='A' && s[i]<='Z'){
            //lettera maiuscola o minuscola
            counter[0]++;
        }else{
            if(s[i]>='0' && s[i]<='9'){
                //numero
                counter[1]++;
            }else{
                if(s[i]==' '){
                    //spazi
                    counter[2]++;
                }else{
                    if(s[i]=='.' || s[i]==',' || s[i]==':' || s[i]==';' || s[i]=='?' || s[i]=='!'){
                        //punteggiatura
                        counter[3]++;
                    }else{
                        //tutto il resto
                        counter[4]++;
                    }
                }
            }      
        }
    }
}

int* processoQ(int from, int to, char* fname){
    char* testo;
    int* stats;
    int i;
    int inizio = from;
    int fine = to;

    testo = malloc(fine*sizeof(char));
    stats = malloc(5*sizeof(int));
    i = 0;

    for(i; i<CLUSTER; i++){
        stats[i] = 0;
    }

    i = readFile(fname, testo, inizio, fine);

    countLetters(fine-inizio, testo, stats);


    if(i==0)
        return stats;
    else 
        return (int *)-1;
}

int* processoQ_n(int from, int to, char** fname, int n){
    char* testo;
    int* stats;
    int i,j, 
        inizio = from, 
        fine = to;

    testo = malloc(fine*sizeof(char));
    stats = malloc(5*sizeof(int));
    i = 0;

    for(i; i<CLUSTER; i++){
        stats[i] = 0;
    }

    for(j=0; j<n; j++){      
        i = readFile(fname[j], testo, inizio, fine);
        countLetters(fine-inizio, testo, stats);
        if(i<0) break;
    }


    if(i==0)
        return stats;
    else 
        return (int *)-1;
}


//---------------------------phil functions------------------------------------


 char **statsToString(int *values){

    char **str = (char **)malloc(CLUSTER * sizeof(char *));
    int i;
    for(i = 0; i < CLUSTER; ++i){
        str[i] = (char *)malloc(12 * sizeof(int));
    }
    
    for( i = 0; i < CLUSTER; ++i){
        sprintf(str[i], "%d", values[i]);
    }

    return str;
}


int *getValuesFromString(char **str){
    int *values = (int *)malloc(CLUSTER * sizeof(int));
    int i;
    for( i = 0; i < CLUSTER; ++i){
        values[i] = atoi(str[i]);
    }

    return values;
}

void printError(int code){
    printf("\n\n----------------------\n\n");
    printf("ERRNO = %d, error description = %s", code, strerror(code));
    printf("\n\n----------------------\n\n");
}

#endif
