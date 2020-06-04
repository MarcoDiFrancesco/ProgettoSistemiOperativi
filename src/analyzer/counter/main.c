#include "counter.h"


int main(int argc, string argv[]) {

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
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

    map fileData = malloc(fileTotal * sizeof(FileMap));
    int i;
    printf("rompicazzo %s\n", files[fileTotal - 1]);
    for (i = 0; i < fileTotal; i++) {
        //printf("file: %s, indice: %d\n", files[i], i);
        fileData[i].name = files[i];
        fileData[i].fileHash = computeHash(files[i], f_dim[i], FALSE);
    }

    /*for(g = 0; g < CLUSTER; g++) {
        data[g] = 0;
    }*/
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
                //printf("sono il P%d e ho preso il file  %s\n con %d file per p\n", i + 1, file_P[k], file_per_p);
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
                int **temp = getValuesFromStringN(buffer, f_Psize);
                storeOnMap(fileData, temp, f_Psize, fileIndex);
                /*for (g = 0; g < CLUSTER; g++) {
                    data[g] += temp[g];
                    //fileData.stats[g] += temp[g];
                }*/

                /*for (g = 0; g < f_Psize; g++) {
                    free(temp[g]);
                } //new: temp non ci serve più perchè il suoi valori vengono passati a dataCollected*/
                free(temp);
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
               
    }

    printf("invio dati...\n");
    //da cancellare solo per testare
    /*map test;
    test=malloc(sizeof(FileMap));
    test[0].name="file1";
    test[0].fileHash=123;
    for(i=0;i<CLUSTER;i++){
        test[0].stats[i]=data[i];
    }*/
    printf("\tfile total dioca %d\n",fileTotal);
    sender(fileData,fileTotal);
    printf("dati inviati!\n");

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return return_value;
}