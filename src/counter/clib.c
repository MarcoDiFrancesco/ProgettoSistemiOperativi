#ifndef _COUNTER_H_
#define _COUNTER_H_

#include "counter.h"


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

    lseek(file, 0, SEEK_END);
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
    int i, 
        inizio = from, 
        fine = to;
    int size;

    testo = malloc(fine*sizeof(char));
    stats = malloc(5*sizeof(int));
    i = 0;

    for(i; i<5; i++){
        stats[i] = 0;
    }

    i = readFile(fname, testo, inizio, fine);

    countLetters(size-inizio, testo, stats);

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

    for(i; i<5; i++){
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

#endif
