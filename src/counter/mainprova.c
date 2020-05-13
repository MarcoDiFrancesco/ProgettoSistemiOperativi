#include "counter.h"

int N=3;
int M=4;

int main(int argc, string argv[]){
    int return_value;
    //temporaneo per testare 
    string files[argc-1];
    int i;
    for(i=1;i<argc;i++){
        files[i-1]=argv[i];
    }
    //tmp
    int file_per_p;
    if((argc - 1)%N == 0){
        file_per_p = (argc - 1)/N;
    }else{
        file_per_p = ((argc - 1)/N) + 1;
    }
    if(argc-1<N){
        N=argc-1;
    }
    int *part = filesPart(files, argc - 1, M);
    int *f_dim = filesDim(files, argc - 1, M);
    //pipes
    int p_c[N][2];
    int q_p[M][2];
    //counters
    int j;
    int data[CLUSTER];
    int g;

    for(g=0;g<CLUSTER;g++){
        data[g]=0;
    }
    printf("Process C pid=%d\n",getpid());
    for(i=0;i<N;i++){
        pipe(p_c[i]);
        pid_t c_son=fork();
        if(c_son==-1){
        printf("error occurred at line 35\n");
        return_value=35;
        }else{
            if(c_son==0){
                return_value=processP(c_son, p_c, q_p, argc, files, M, part, f_dim, i,file_per_p);
                exit(return_value);
            }else{
                //successive parti del processo C
                string * buffer=readAndWait(p_c[i],c_son);
                int *tmp=getValuesFromString(buffer);
                for(g=0;g<CLUSTER;g++){
                     data[g]+=tmp[g];
                    }
                }
        }
    }
    printf("Printing data....\n");
    printf("Numero di lettere calcolato= %d\n",data[0]);
    printf("Numero di numeri calcolato= %d\n",data[1]);
    printf("Numero di spazi calcolato= %d\n",data[2]);
    printf("Numero di punteggiatura calcolato= %d\n",data[3]);
    printf("Numero di altro calcolato= %d\n",data[4]);

    free(part);
    free(f_dim);

    return return_value;
}