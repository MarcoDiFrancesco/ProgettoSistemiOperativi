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

int * filesPart(string *files,int num,int M){
    int i;
    string toRead;
    int fp,dim;
    int *ret;
    ret=malloc(num*sizeof(int));
    for(i=0;i<num;i++){
        toRead=files[i];
        fp=open(toRead,O_RDONLY);
        dim = lseek(fp,0,SEEK_END);
        if (dim%M == 0){
            ret[i]=dim/M;
        } else {
            ret[i] = (dim/M) + 1;
        }
        close(fp);
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

int* processoQ_n(int *range, int *dims, char** fname, int n, int q_loop, int index, int M){
    char* testo;
    int* stats;
    int i,j, inizio[n], fine[n];
    i = 0;

    for (j = index; j < (index + n); j++) {

        inizio[i] = range[j]*q_loop;
        if((range[j]*(q_loop + 1) <= dims[j])) {
            // printf("%d",range[j]*q_loop + j);
            fine[i] = range[j]*(q_loop + 1);
        } else {
            fine[i] = dims[j]; 
        } 

        if(inizio[i] > fine[i]){
            inizio[i] = fine[i] = 0;
        }   
    
        if(inizio[i] != fine[i]) printf("\tinizio=%d, fine=%d\n",inizio[i],fine[i]);
        else printf("\t---------\n");
        ++i;
    }

    testo = malloc(MAXLEN*sizeof(char));
    stats = malloc(5*sizeof(int));
    i = 0;

    for(i; i<CLUSTER; i++){
        stats[i] = 0;
    }

    for(j=0; j<n; j++){  
        i = readFile(fname[j], testo, inizio[j], fine[j]);
        //printf("\t%s\n",testo);
        countLetters(fine[j]-inizio[j], testo, stats);
        if(i<0) break;
    }
    //free(testo);


    if(i==0)
        return stats;
    else 
        return (int *)-1;
}


//---------------------------phil functions------------------------------------


 char **statsToString(int *values) {

    char **str = (char **)malloc(CLUSTER * sizeof(char *));
    int i;
    for(i = 0; i < CLUSTER; ++i) {
        str[i] = (char *)malloc(12 * sizeof(int));
    }
    
    for(i = 0; i < CLUSTER; ++i) {
        sprintf(str[i], "%d", values[i]);
    }

    return str;
}


int *getValuesFromString(char **str) {
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

int *filesDim(string *files,int num,int M) {
    int i;
    string toRead;
    int fp;
    int *ret;
    ret=malloc(num*sizeof(int));
    for(i=0;i<num;i++){
        toRead=files[i];
        fp=open(toRead,O_RDONLY);
        ret[i] = lseek(fp, 0, SEEK_END);
        close(fp);
    }
    return ret;
}

//------------------------ process function section-----------------------


int processP(pid_t c_son, int pipe_c[][2], int pipe_q[][2], int argc, string files[], int m, int *part, int *fdim, int index_p,int file_per_p){
    //creo pipe fra C e P
    int g;
    int return_value;
        if(c_son==0){
            //processo P
            string *qTop[m];
            int dataCollected[CLUSTER];
            for(g=0;g<CLUSTER;g++){
                dataCollected[g]=0;
            }
        printf("P created pid=%d ppid=%d\n",getpid(),getppid());                
        int k = 0;
        string file_P[file_per_p];
        int f_Psize=0;
        while(k < file_per_p){
            if((file_per_p * index_p) + k < argc - 1){
                file_P[k] = files[(file_per_p * index_p) + k];
                f_Psize++;
            }
            if((file_per_p * index_p) + k == argc - 1){
                    file_P[k] = 0;
                }
                ++k;
            }
        int j;
        //creo M processi di tipo Q
        for(j=0;j<m;j++){
            //creo la pipe fra P e Q
            pipe(pipe_q[j]);
            int p_son=fork();
            if(p_son==-1){
                printf("error occurred at line 46\n");
                return_value=46;
            }else{
                if(p_son==0){
                    return_value=processQ(part, fdim, file_P,f_Psize,j,file_per_p*index_p, m, pipe_q[j]);
                    exit(0);
                }else{
                    //successive parti del processo P
                    qTop[j]=readAndWait(pipe_q[j],p_son);
                    int *tmp=getValuesFromString(qTop[j]);
                    for(g=0;g<CLUSTER;g++){
                        dataCollected[g]+=tmp[g];
                    }
                }
        }
                        
    }
    return_value=writePipe(pipe_c[index_p],statsToString(dataCollected));
    return return_value;
    }
}

int processQ(int *range, int *dims, char** fname, int f_Psize, int q_loop, int index, int m, int pipe_q[]){
    printf("\tQ created pid=%d ppid=%d\n",getpid(),getppid());
    int* counter=processoQ_n(range, dims, fname,f_Psize,q_loop,index, m);
    string *message=statsToString(counter);
    int err=writePipe(pipe_q,message);
    return err;
}
#endif
