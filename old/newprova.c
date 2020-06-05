#include "counternew.h"

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
        if (argv[i][lunghezza_nome - 4] == '.' && argv[i][lunghezza_nome - 3] == 't' && 
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
    if (fileTotal < 1) {
        printf("non ho ricevuto alcun file valido\n");
        exit(0);
    }
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

    map fileData[fileTotal];
    for (i = 0; i < fileTotal; ++i) {
        fileData[i] = (map)malloc(sizeof(struct FileMap));
        fileData[i]->name = files[i];
        fileData[i]->fileHash = computeHash(files[i], f_dim[i], FALSE);
    }

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

        int k = 0;
        string file_P[file_per_p];
        int f_Psize = 0;
        int fileIndexTemp = fileIndex;
        while (k < file_per_p) {
            if (fileIndexTemp < fileTotal) {
                file_P[k] = files[fileIndexTemp++];
                f_Psize++;
                //printf("sono il P%d e ho preso il file  numero %d\n con %d file per p\n", index_p, fileIndexTemp - 1, file_per_p);
            }
            if (fileIndexTemp - 1 == fileTotal) {
                file_P[k] = 0;
                //printf("ho finito i file\n");
                }
            ++k;
        }

        pipe(p_c[i]);
        pid_t c_son = fork();
        if (c_son == -1) {
            printf("error occurred at line 35\n");
            return_value=35;
        } else {
            if (c_son == 0) {
                return_value = processP(c_son, p_c, q_p, file_P, N, M, 
                                        fileTotal, fileIndex, part, f_dim, 
                                        i, file_per_p, f_Psize); 
                exit(return_value);
            } else {
                //successive parti del processo C
                string **buffer = readAndWaitN(p_c[i], c_son, f_Psize);
                int **tmp = getValuesFromStringN(buffer, f_Psize);
                storeOnMap(fileData, tmp, f_Psize, fileIndex);
                /*for (g = 0; g < CLUSTER; g++) {
                    data[g] += tmp[g];
                    //fileData->stats[g] += tmp[g];
                }*/
                for (g = 0; g < fileTotal; g++) {
                    free(tmp[g]);
                }
            }
        }
        fileIndex += file_per_p;
        file_restanti -= file_per_p;
        //printf("sto valore stupido è %d\n", N - i - 1);
        if (i != N - 1) {
            file_per_p = ceiling(file_restanti, N - i - 1);
        }
                
    }

    //sleep(10);
    printf("Printing data....\n");
    /*printf("Numero di lettere calcolato= %d\n", data[0]);
    printf("Numero di numeri calcolato= %d\n", data[1]);
    printf("Numero di spazi calcolato= %d\n", data[2]);
    printf("Numero di punteggiatura calcolato= %d\n", data[3]);
    printf("Numero di altro calcolato= %d\n", data[4]);*/

    printf("\nHo analizzato i seguenti files:\n\n");
    for (i = 0; i < fileTotal; ++i) {
        printf("%s\n", fileData[i]->name);
        if (fileData[i]->fileHash != computeHash(files[i], f_dim[i], TRUE)) {
            printf("WARNING: %s has been modified while being analyzed,\nthis data is about the file prior to modification\n", fileData[i]->name);
        }
        printf("Old hash= %lu, new hash= %lu\n", fileData[i]->fileHash, computeHash(files[i], f_dim[i], TRUE));
        printf("\tNumero di lettere calcolato= %d\n", fileData[i]->stats[0]);
        printf("\tNumero di numeri calcolato= %d\n", fileData[i]->stats[1]);
        printf("\tNumero di spazi calcolato= %d\n", fileData[i]->stats[2]);
        printf("\tNumero di punteggiatura calcolato= %d\n", fileData[i]->stats[3]);
        printf("\tNumero di altro calcolato= %d\n", fileData[i]->stats[4]); 
               
    }


    //printf("invio dati...\n");
    //sender(data);
    //printf("dati inviati!\n");

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return return_value;
}