#include "counter.h"

int main(int argc, string argv[]) {
    BOOL flagMain=FALSE;
    if(strcmp(argv[argc-1], "-a")){
        //flag main setted
        flagMain=TRUE;
    }

    
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
            return_value = 35;
        } else {
            if (PIds[i] == 0) {
                PIds[i] = getpid();
                return_value = processP(PIds[i], p_c, q_p, file_P,
                                        fileTotal, fileIndex, part, f_dim,
                                        i, file_per_p, f_Psize[i]);
                if (kill(getppid(), SIGUSR2) == 0) 
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

    printf("Sto analizzando");
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

    sendConfirm("r", 6, "/root/src/main/main.c");
    fileIndex = 0;
    for (i = 0; i < N; i++) {
        string **buffer = readAndWaitN(p_c[i], f_Psize[i]);
        int **temp = getValuesFromStringN(buffer, f_Psize[i]);
        storeOnMap(fileData, temp, f_Psize[i], fileIndex);
        free(temp);
        fileIndex += f_Psize[i];
    }
    free(f_Psize);
    string back = recConfirm(3, PathName2);
    while (strcmp(back, "y") != 0 && strcmp(back, "n") != 0) {
        back = recConfirm(3, PathName2);
    }
    if (strcmp(back, "y") == 0) {
        return 0;
    }

    for (i = 0; i < fileTotal; ++i) {
        if (fileData[i].fileHash != computeHash(files[i], f_dim[i], TRUE)) {
            printf("WARNING: %s has been modified while being analyzed,\nthis data is about the file prior to modification\n", fileData[i].name);
        }
    }
    printf("Numero di file analizzati: %d\n", fileTotal);
    if(flagMain==TRUE){
        kill(getppid(), SIGUSR1);
    }
    sender(fileData, fileTotal);

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return return_value;
}