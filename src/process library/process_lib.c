#include "process_lib.h"

void c(int argc, string argv[], int N, int M){

    //processo C
    printf("Process C pid=%d\n",getpid());

    //vars
    int return_value;
    string toRead=argv[1];
    pid_t c_son;
    int ret = 0;
        //pipes
    int pipeCP[N][2];
    int pipePQ[M][2];
        //counters
    int counterP;
    int counterQ;
    int data[CLUSTER];
    int g;

    //controllo dimensione del file
    int fp=open(toRead,O_RDONLY);
    int dim = lseek(fp,0,SEEK_END);
    int dimpart=dim/M;

    //inizializzazione vettore per recupero dati
    for(g=0;g<CLUSTER;g++){
        data[g]=0;
    }

    //temporaneo per testare 
    string files[argc-1];
    int i;
    for(i=1;i<argc;i++){
        files[i-1]=argv[i];
    }
    
    //creazione di N processi P
    for(counterP=0; counterP<N; counterP++){
        //creazione delle pipe tra C e P
        set_err_msg(&ret, pipe(pipeCP[counterP]),"line 30 (P) --> errore nella creazione della pipe");
        c_son=fork();
        set_err_msg(&ret, c_son, "line 32 (P) --> figlio non creato");
        if(ret >= 0){
            if(c_son == 0){
                p(M, pipePQ, pipeCP, counterP, dimpart, files);
            } else{
                //chiusura processi P
                string * buffer=readAndWait(pipeCP[i],c_son);
                //funzione per raggruppare e sommre i dati
                int *tmp=getValuesFromString(buffer);
                for(g=0;g<CLUSTER;g++){
                    data[g]+=tmp[g]; 
                }
            }
        }
    }
    
    //stampa temporanea dei dati
    printf("Printing data....\n");
    printf("Numero di lettere calcolato= %d\n",data[0]);
    printf("Numero di numeri calcolato= %d\n",data[1]);
    printf("Numero di spazi calcolato= %d\n",data[2]);
    printf("Numero di punteggiatura calcolato= %d\n",data[3]);
    printf("Numero di altro calcolato= %d\n",data[4]);
}

p(int M, int pipePQ[][2], int pipeCP[][2], int n_proc, int dimpart, string files[]){
    //processo P
    printf("P created pid=%d ppid=%d\n",getpid(),getppid());

    //vars
    int g = 0;
    int process_n = 0;
    pid_t p_son;
    int ret = 0;

    //funzione per raggruppare e sommre i dati
    string *qTop[M];
    int dataCollected[CLUSTER];
    for(g=0; g<CLUSTER; g++){
        dataCollected[g] = 0;
    }

    //creazione di M processi Q
    for(process_n=0; process_n<M; process_n++){
        //creazione della pipe tra P e Q
        set_err_msg(&ret, pipe(pipePQ[process_n]),"line 30 (P) --> errore nella creazione della pipe");
        p_son=fork();
        set_err_msg(&ret, p_son, "line 32 (P) --> figlio non creato");
        if(ret >= 0){
            if(p_son == 0){
                //esecuzione del processo Q
                set_err_msg(&ret, q(pipePQ, dimpart, process_n, files), "line 36 (P) --> errore nel processo Q");
            } else{
                //funzione per raggruppare e sommre i dati
                qTop[process_n]=readAndWait(pipePQ[process_n],p_son);
                int *tmp=getValuesFromString(qTop[process_n]);
                for(g=0;g<CLUSTER;g++){
                     dataCollected[g]+=tmp[g];               
                }
            }
        }
    }

    //potenziale uscita del processo P
    set_err_msg(&ret, writePipe(pipeCP[n_proc],statsToString(dataCollected)), "line 49 (P) --> errore nella scrittura della pipe");
    return ret;
}

q(int pipePQ[][2], int dimpart, int section, string files[]){
    //processo Q
    printf("\tQ created pid=%d ppid=%d\n",getpid(),getppid());

    //vars
    int ret = 0;

    //logica processo Q
    int* counter=processoQ(dimpart*section, dimpart*(section+1),files[0]);
    string *message=statsToString(counter);
    set_err_msg(&ret, writePipe(pipePQ ,message), "line 18 (Q) --> pipe non scritta correttamente");
    return ret;
}