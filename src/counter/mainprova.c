#include "counter.h"


int main(int argc, string argv[]) {
    printf("richi scemo chi legge\n");
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int fileTotal = atoi(argv[3]);
    printf("tot = %d", fileTotal);
    string files[fileTotal];
    int j;
    for(j=0; j<fileTotal; j++){
        files[j]=malloc(sizeof(char)*100);
        strcpy(files[j], argv[j+4]);
        printf("cazzi %d\n",j);
    }
    printf("richi scemo chi legge\n");
    int fileIndex = 0;
    int file_restanti = fileTotal;
    int file_per_p = ceiling(fileTotal, N);
    int return_value;
    //pipes
    int p_c[N][2];
    int q_p[M][2];
    //counters
    int data[CLUSTER];
    int g;

    int *part = filesPart(files, fileTotal, M);
    int *f_dim = filesDim(files, fileTotal);


    for(g = 0; g < CLUSTER; g++) {
        data[g] = 0;
    }
    printf("\n\nProcess C pid=%d\n",getpid());
    int i;
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