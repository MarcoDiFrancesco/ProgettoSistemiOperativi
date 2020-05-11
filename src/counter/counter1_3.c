#include "counter.h"

int N=3;
int M=4;

int main(int argc, string argv[]){

    int return_value;
    //string toRead=argv[1];

    //temporaneo per testare 
    string files[argc-1];
    int i;
    for(i=1;i<argc;i++){
        files[i-1]=argv[i];
    }
    //tmp

<<<<<<< HEAD
    int file_per_p;
    if((argc - 1)%N == 0){
        //printf("areo1\n");
        file_per_p = (argc - 1)/N;
=======
    //int file_per_p = ceil((float)(argc - 1)/(float)N);
    int file_per_p = ((argc - 1)/N) + 1;
    /*if(N == argc - 1){
        file_per_p = 1;
>>>>>>> fa99207a113df68875a59ab70e1b8f2b10ee15db
    } else {
        //printf("areo2\n");
        file_per_p = ((argc - 1)/N) + 1;
    }

    if(argc-1<N){
        N=argc-1;
    }

    int *part = filesDim(files, argc-1,M);
    /*
    int fp=open(toRead,O_RDONLY);
    int dim = lseek(fp,0,SEEK_END);
    int part=dim/M;*/
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
        //creo N processi di tipo P
        //creo pipe fra C e P
        pipe(p_c[i]);
        pid_t c_son=fork();
        if(c_son==-1){
            printf("error occurred at line 35\n");
            return_value=35;
        }else{
            if(c_son==0){
                //processo P
                string *qTop[M];
                int dataCollected[CLUSTER];
                for(g=0;g<CLUSTER;g++){
                    dataCollected[g]=0;
                }
                printf("P created pid=%d ppid=%d\n",getpid(),getppid());

                
                int k = 0;
                string file_P[file_per_p];
                int f_Psize=0;
                while(k < file_per_p){
                    if((file_per_p * i) + k < argc - 1){
                        file_P[k] = files[(file_per_p * i) + k];
                        f_Psize++;
                    }
                    if((file_per_p * i) + k == argc - 1){
                        file_P[k] = 0;
                    }
                    ++k;
                }

                //creo M processi di tipo Q
                for(j=0;j<M;j++){

                    //creo la pipe fra P e Q
                    pipe(q_p[j]);
                    int p_son=fork();
                    if(p_son==-1){
                        printf("error occurred at line 46\n");
                        return_value=46;
                    }else{
                        if(p_son==0){
                            //processo Q
                            printf("\tQ created pid=%d ppid=%d\n",getpid(),getppid());

                            //logica processo  q
                            int* counter=processoQ_n(part,file_P,f_Psize,j,file_per_p*i);
                            string *message=statsToString(counter);
                            int err=writePipe(q_p[j],message);
                            exit(0);
                        }else{
                            //successive parti del processo P
                            qTop[j]=readAndWait(q_p[j],p_son);
                            int *tmp=getValuesFromString(qTop[j]);
                            for(g=0;g<CLUSTER;g++){
                                dataCollected[g]+=tmp[g];
                            }
                        }
                        
                    }
                }
                //potenziale uscita del processo P
                writePipe(p_c[i],statsToString(dataCollected));
                exit(0);
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

    return return_value;
}