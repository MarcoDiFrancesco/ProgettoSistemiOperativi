#include "counter.h"

int N = 3;
int M = 4;
int n_arg = 1;

int main(int argc, string argv[]) {
    if (argc == 3) 
        printf(">.<");
    //--------------------------------------------------
    //--------------------------------------------------
    //prova arg N M

    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        printf("N1");
        n_arg++;
        N = atoi(argv[2]);
        n_arg++;
        } else if (argc > 4 && strcmp(argv[3], "-n") == 0) {
            printf("N2");
            n_arg++;
            N = atoi(argv[4]);
            n_arg++;
        }
    
    if (argc > 2 && strcmp(argv[1], "-m") == 0) {
        printf("M1");
        n_arg++;
        M = atoi(argv[2]);
        n_arg++;
        } else if (argc > 4 && strcmp(argv[3], "-m") == 0) {
            printf("M2");
            n_arg++;
            M = atoi(argv[4]);
            n_arg++;
        }

    int a;
    printf("\nfiles inseriti =  %d", argc - n_arg);
    for (a = n_arg; a<argc; a++) {
        printf("\nfile %d = %s", argc - a, argv[a]);
    }

    printf("\n(numero di argomenti inseriti prima dei file %d) \nN: %d\nM: %d\n", 
               n_arg - 1, N, M);

    //controllo sul nome dei file passati come argomento
    BOOL filesOk[argc - n_arg];
    int fileErrati = 0;
    int i;
    printf("nome corretto: ");
    for (i = n_arg; i < argc; i++) {
        int lunghezza_nome = strlen(argv[i]);
        if(argv[i][lunghezza_nome - 4] == '.' && argv[i][lunghezza_nome - 3] == 't' && 
           argv[i][lunghezza_nome - 2] == 'x' && argv[i][lunghezza_nome - 1] == 't') {
            filesOk[i] = TRUE;
            printf(" OK ");
        } else {
            filesOk[i] = FALSE;
            fileErrati++;
            printf(" NO ");
        }
    }

    // Controllo sull'esistenza dei file passati
    int fileInesistenti = 0;
    puts("\n\nControllo l'esistenza dei file validi\n");
    for (i = n_arg; i < argc ; ++i) {
        if (filesOk[i]) {
            if (access(argv[i], F_OK) == -1) {
                filesOk[i] = FALSE;
                fileInesistenti++;
            }
        }
    }


    int return_value;
    int const fileTotal = argc - n_arg - fileErrati - fileInesistenti;
<<<<<<< HEAD
=======
    if (fileTotal < 1) {
        printf("non ho ricevuto alcun file valido\n");
        exit(0);
    }
>>>>>>> 851c60ea93c2946f6e44ebed784c04052e103c95
    //temporaneo per testare 
    string files[fileTotal];
    printf(" files OK = %d\n", fileTotal);
    int next = 0;
    for (i = n_arg; i < argc; i++) {
        if (filesOk[i] == TRUE) {
            files[next] = argv[i];
            next++;
        }   
    }
    //tmp

<<<<<<< HEAD
    if (fileTotal < 1) {
        printf("non ho ricevuto alcun file valido\n");
        exit(0);
    }
    
=======
>>>>>>> 851c60ea93c2946f6e44ebed784c04052e103c95
    int fileIndex = 0;
    int file_restanti = fileTotal;
    int file_per_p = ceiling(fileTotal, N);
    
    if (fileTotal < N) {
        N = fileTotal;
    }

    int *part = filesPart(files, fileTotal, M);
    int *f_dim = filesDim(files, fileTotal);

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

    for(g = 0; g < CLUSTER; g++) {
        data[g] = 0;
    }
    printf("\n\nProcess C pid=%d\n",getpid());

    for (i = 0; i < N; i++) {
        //printf("file restanti: %d --- processi restanti: %d\n", file_restanti, (N - i));
        //printf("questo proceso legge: %d files\n\n", file_per_p);
        pipe(p_c[i]);
        pid_t c_son = fork();
        if (c_son == -1) {
            printf("error occurred at line 35\n");
            return_value=35;
        } else {
            if (c_son == 0) {
                return_value = processP(c_son, p_c, q_p, files, N, M, 
                                        fileTotal, fileIndex, part, f_dim, 
                                        i, file_per_p); 
                exit(return_value);
            } else {
                //successive parti del processo C
                string * buffer = readAndWait(p_c[i],c_son);
                int *tmp = getValuesFromString(buffer);
                for (g = 0; g < CLUSTER; g++) {
                    data[g] += tmp[g];
                }
                free(tmp); //new: tmp non ci serve più perchè il suoi valori vengono passati a dataCollected
            }
        }
        fileIndex += file_per_p;
        file_restanti -= file_per_p;
        //printf("sto valore stupido è %d\n", N - i - 1);
        if (i != N - 1) {
            file_per_p = ceiling(file_restanti, N - i - 1);
        }
                
    }
    printf("Printing data....\n");
    printf("Numero di lettere calcolato= %d\n", data[0]);
    printf("Numero di numeri calcolato= %d\n", data[1]);
    printf("Numero di spazi calcolato= %d\n", data[2]);
    printf("Numero di punteggiatura calcolato= %d\n", data[3]);
    printf("Numero di altro calcolato= %d\n", data[4]);


    printf("invio dati...\n");
    sender(data);
    printf("dati inviati!\n");

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return return_value;
}