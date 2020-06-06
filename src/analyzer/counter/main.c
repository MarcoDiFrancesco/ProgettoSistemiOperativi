#include "counter.h"

int main(int argc, string argv[]) {
    int k = 0;

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    PIds=malloc(sizeof(pid_t)*N);
    QIds=malloc(N * sizeof(pid_t *));
    for(k = 0; k < N; ++k) {
        QIds[k] = malloc(M * sizeof(pid_t));
    }
    boolQ=malloc(sizeof(BOOL)*N);

    int fileTotal = atoi(argv[3]);

    string files[fileTotal];
    int j;
    for(j=0; j<fileTotal; j++){
        files[j]=malloc(sizeof(char)*100);
        strcpy(files[j], argv[j+4]);
    }
    int fileIndex = 0;
    int file_restanti = fileTotal;
    int file_per_p = ceiling(fileTotal, N);
    int return_value;
    //pipes
    int p_c[N][2];
    int q_p[M][2];
    //counters
    //int data[CLUSTER];
    int g;

    int *part = filesPart(files, fileTotal, M);
    int *f_dim = filesDim(files, fileTotal);
    int *f_Psize = malloc(N * sizeof(int));
    for (k = 0; k < N; ++k) {
        f_Psize[k] = 0;
    }

    map fileData = malloc(fileTotal * sizeof(FileMap));
    int i;
    for (i = 0; i < fileTotal; i++) {
        fileData[i].name = files[i];
        fileData[i].fileHash = computeHash(files[i], f_dim[i], FALSE);
    }

    printf("\n\nProcess C pid=%d\n",getpid());
    for (i = 0; i < N; i++) {
        int k = 0;
        string file_P[file_per_p];
        int fileIndexTemp = fileIndex;
        while (k < file_per_p) {
            if (fileIndexTemp < fileTotal) {
                file_P[k] = files[fileIndexTemp++];
                f_Psize[i]++;
            }
            if (fileIndexTemp - 1 == fileTotal) {
                file_P[k] = 0;
                }
            ++k;
        }
        
        pipe(p_c[i]);
        PIds[i] = fork();
        if (PIds[i] == -1) {
            printf("error occurred at line 35\n");
            return_value=35;
        } else {
            if (PIds[i] == 0) {
                return_value = processP(PIds[i], p_c, q_p, file_P, N, M, 
                                        fileTotal, fileIndex, part, f_dim, 
                                        i, file_per_p, f_Psize[i]);                
                kill(getppid(), SIGUSR2);
                exit(return_value);
            } else {
                //successive parti del processo C
                
            }
        }
        fileIndex += file_per_p;
        file_restanti -= file_per_p;
        if (i != N - 1) {
            file_per_p = ceiling(file_restanti, N - i - 1);
        }
                
    }

    while(boolP==FALSE){
        system("sleep 1");
    }
    fileIndex = 0;
    for (i = 0; i < N; i++) {
        string **buffer = readAndWaitN(p_c[i], f_Psize[i]);
        int **temp = getValuesFromStringN(buffer, f_Psize[i]);
        storeOnMap(fileData, temp, f_Psize[i], fileIndex);
        free(temp);
        fileIndex += f_Psize[i];
    }

    printf("Printing data....\n");
    printf("\nHo analizzato i seguenti files:\n\n");
    for (i = 0; i < fileTotal; ++i) {
        printf("%s\n", fileData[i].name);
        if (fileData[i].fileHash != computeHash(files[i], f_dim[i], TRUE)) {
            printf("WARNING: %s has been modified while being analyzed,\nthis data is about the file prior to modification\n", fileData[i].name);
        }
        printf("Old hash= %lu, new hash= %lu\n", fileData[i].fileHash, computeHash(files[i], f_dim[i], TRUE));
        printf("\tNumero di lettere calcolato= %d\n", fileData[i].stats[0]);
        printf("\tNumero di numeri calcolato= %d\n", fileData[i].stats[1]);
        printf("\tNumero di spazi calcolato= %d\n", fileData[i].stats[2]);
        printf("\tNumero di punteggiatura calcolato= %d\n", fileData[i].stats[3]);
        printf("\tNumero di altro calcolato= %d\n", fileData[i].stats[4]); 
        /*if(fileData[i].stats[0] + fileData[i].stats[1] + fileData[i].stats[2] + fileData[i].stats[3] + fileData[i].stats[4] !=
        f_dim[i]){
            puts("\n\n TANTI CAZZI \n\n");
        }*/
    }

    printf("invio dati...\n");
    printf("\tfile total %d\n",fileTotal);
    //sender(fileData,fileTotal);
    printf("dati inviati!\n");

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return return_value;
}