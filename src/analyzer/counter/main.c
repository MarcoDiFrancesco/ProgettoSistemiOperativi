#include "counter.h"

int main(int argc, string argv[]) {
    BOOL flagMain=FALSE;
    if(strcmp(argv[argc-1], "-a")){
        //flag main setted
        flagMain=TRUE;
    }

    printf("start counter\n");
    if (argc < 4) {
        printf("Counter chiamato con argomenti insufficienti\n");
        return 1;
    }

    signal(SIGUSR2, sighandlerP);
    int k, b = 0;
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    PIds = malloc(N * sizeof(pid_t));
    QIds = malloc(N * sizeof(pid_t *));
    for (k = 0; k < N; ++k) {
        PIds[k] = 0;
        QIds[k] = malloc(M * sizeof(pid_t));
        for (b = 0; b < M; ++b) {
            QIds[k][b] = 0;
        }
    }
    boolQ = malloc(sizeof(BOOL) * N);
    for (k = 0; k < N; ++k) {
        boolQ[k] = FALSE;
    }
    boolP = FALSE;

    checkQ = malloc(sizeof(int) * N);
    for (k = 0; k < N; k++) {
        checkQ[k] = 0;
    }
    checkP = 0;

    int fileTotal = atoi(argv[3]);

    string files[fileTotal];
    int j;
    for (j = 0; j < fileTotal; j++) {
        files[j] = malloc(sizeof(char) * 100);
        strcpy(files[j], argv[j + 4]);
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

    //printf("\n\nProcess C pid=%d\n",getpid());
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
        //printf("qui pids[%d]=%d e N=%d\n", i, PIds[i], N);
        if (PIds[i] == -1) {
            printf("error occurred at line 35\n");
            return_value = 35;
        } else {
            if (PIds[i] == 0) {
                PIds[i] = getpid();
                return_value = processP(PIds[i], p_c, q_p, file_P,
                                        fileTotal, fileIndex, part, f_dim,
                                        i, file_per_p, f_Psize[i]);
                if (kill(getppid(), SIGUSR2) == 0) {
                    //printf("P ha mandato una signal qid : %d\n", getpid());
                } else {
                    //printf("!!!!Perror  %s\n", strerror(errno));
                }

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

    printf("Sto analizzando...");
    while (boolP == FALSE) {
        system("sleep 1");
        printf(".");
        fflush(stdout);
        int tmp_counter = 0;
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                if (waitpid(QIds[i][j], NULL, WNOHANG) == QIds[i][j]) {
                    tmp_counter++;
                }
            }
        }
        if (tmp_counter == N * M)
            boolP = TRUE;
    }

    fileIndex = 0;
    for (i = 0; i < N; i++) {
        string **buffer = readAndWaitN(p_c[i], f_Psize[i]);
        int **temp = getValuesFromStringN(buffer, f_Psize[i]);
        storeOnMap(fileData, temp, f_Psize[i], fileIndex);
        free(temp);
        fileIndex += f_Psize[i];
    }
    free(f_Psize);
    string back = recConfirm(3);
    while (strcmp(back, "y") != 0 && strcmp(back, "n") != 0) {
        system("sleep 1");
        back = recConfirm(3);
    }
    printf("Ho ricevuto gli ordini da analyzer\n");
    if (strcmp(back, "y") == 0) {
        puts("lol mi ammazzo");
        exit(0);
    }
    /*
    msgsnd(ho finito con nowait);
    msgrcv(cosa devo fare? )
    if (ho file nuovi) {
        msgsnd(mi ammazzo);
        exit(0);
    }
    */

    //printf("Printing data....\n");
    //printf("\nHo analizzato i seguenti files:\n\n");
    for (i = 0; i < fileTotal; ++i) {
        //printf("%s\n", fileData[i].name);
        if (fileData[i].fileHash != computeHash(files[i], f_dim[i], TRUE)) {
            printf("WARNING: %s has been modified while being analyzed,\nthis data is about the file prior to modification\n", fileData[i].name);
        }
        /*printf("Old hash= %lu, new hash= %lu\n", fileData[i].fileHash, computeHash(files[i], f_dim[i], TRUE));
        printf("\tNumero di lettere maiuscole calcolato= %d\n", fileData[i].stats[UPPERCASE]);
        printf("\tNumero di lettere minuscole calcolato= %d\n", fileData[i].stats[LOWERCASE]);
        printf("\tNumero di numeri calcolato= %d\n", fileData[i].stats[NUMBERS]);
        printf("\tNumero di spazi calcolato= %d\n", fileData[i].stats[SPACES]);
        printf("\tNumero di punteggiatura calcolato= %d\n", fileData[i].stats[PUNCTUATION]);
        printf("\tNumero di parentesi= %d\n", fileData[i].stats[PARENTHESIS]);
        printf("\tNumero di operatori matematici= %d\n", fileData[i].stats[MATH_OPERATORS]);
        printf("\tNumero di altro calcolato= %d\n", fileData[i].stats[OTHER]); */
        /*if(fileData[i].stats[0] + fileData[i].stats[1] + fileData[i].stats[2] + fileData[i].stats[3] + fileData[i].stats[4] !=
        f_dim[i]){
            puts("\n\n TANTI CAZZI \n\n");
        }*/
    }

    //printf("invio dati...\n");
    printf("\n\n");
    printf("Numero di file analizzati: %d\n", fileTotal);
    if(flagMain==TRUE){
        kill(getppid(), SIGUSR1);
        printf("sigsent\n");
    }else{
        printf("Per inviare i dati è necessario lanciare il report [bin/report]\n");
    }
    sender(fileData, fileTotal);
    printf("Dati inviati con successo!\n");

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return return_value;
}