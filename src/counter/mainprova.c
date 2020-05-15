#include "counter.h"

int N=3;
int M=4;
int n_arg = 1;

int main(int argc, string argv[]){
    printf("> ");
    //--------------------------------------------------
    //--------------------------------------------------
    //prova arg N M

    if(strcmp(argv[1], "-n") == 0) {
        printf("N1");
        n_arg++;
        N = atoi(argv[2]);
        n_arg++;
        } else if(strcmp(argv[3], "-n") == 0) {
            printf("N2");
            n_arg++;
            N = atoi(argv[4]);
            n_arg++;
        }
    
    if(strcmp(argv[1], "-m") == 0){
        printf("M1");
        n_arg++;
        M = atoi(argv[2]);
        n_arg++;
        } else if(strcmp(argv[3], "-m") == 0) {
            printf("M2");
            n_arg++;
            M = atoi(argv[4]);
            n_arg++;
        }

    int a;
    printf("\nfiles inseriti =  %d", argc - n_arg);
    for(a = n_arg; a<argc; a++){
        printf("\nfile %d = %s", argc - a, argv[a]);
    }

    printf("\n(numero di argomenti inseriti prima dei file %d) \nN: %d\nM: %d\n", n_arg-1, N, M);

    //controllo sul nome dei file passati come argomento
    BOOL filesOk[argc-n_arg];
    int fileErrati = 0;
    int i;
    printf("nome corretto: ");
    for(i=n_arg; i<argc; i++){
        int lunghezza_nome = sizeof(argv[i]);
        if(argv[i][lunghezza_nome-4] == '.' && argv[i][lunghezza_nome-3] == 't' && argv[i][lunghezza_nome-2] == 'x'&& argv[i][lunghezza_nome-1] == 't'){
            filesOk[i] = TRUE;
            printf(" OK ");
        } else {
            filesOk[i] = FALSE;
            fileErrati++;
            printf(" NO ");
        }
    }

    int return_value;
    //temporaneo per testare 
    string files[argc - n_arg - fileErrati];
    printf(" files OK = %d\n", argc - n_arg - fileErrati);
    int next = 0;
    for(i=n_arg;i<argc;i++){
        if(filesOk[i] == TRUE){
            files[next]=argv[i];
            next++;
        }   
    }
    //tmp
    
    int file_per_p;
    if((argc - n_arg - fileErrati)%N == 0){
        file_per_p = (argc - n_arg - fileErrati)/N;
    } else {
        file_per_p = ((argc - n_arg - fileErrati)/N) + 1;
    }
    
    if (argc - n_arg - fileErrati < N) {
        N = argc - n_arg - fileErrati;
    }

    int *part = filesPart(files, argc - n_arg - fileErrati, M);
    int *f_dim = filesDim(files, argc - n_arg - fileErrati, M);

    //fine prova
    //--------------------------------------------------
    //--------------------------------------------------


    /*SICURO CHE VA
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
    } else {
        file_per_p = ((argc - 1)/N) + 1;
    }
    if (argc - 1 < N) {
        N = argc - 1;
    }
    
    
    int *part = filesPart(files, argc - 1, M);
    int *f_dim = filesDim(files, argc - 1, M);
    */

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
    printf("\n\nProcess C pid=%d\n",getpid());

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
                free(tmp); //new: tmp non ci serve più perchè il suoi valori vengono passati a dataCollected
                }
        }
    }
    printf("Printing data....\n");
    printf("Numero di lettere calcolato= %d\n",data[0]);
    printf("Numero di numeri calcolato= %d\n",data[1]);
    printf("Numero di spazi calcolato= %d\n",data[2]);
    printf("Numero di punteggiatura calcolato= %d\n",data[3]);
    printf("Numero di altro calcolato= %d\n",data[4]);

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return return_value;
}