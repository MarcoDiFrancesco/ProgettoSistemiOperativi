#include "test.h"

int N = 3;
int M = 4;
int n_arg = 1;
string nS="3";
string mS="4";

int main(int argc, string argv[]) {
    //scrivo qui controllo degli argomenti per il C
    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        printf("N1");
        n_arg++;
        nS = argv[2];
        N = atoi(argv[2]);
        n_arg++;
        } else if (argc > 4 && strcmp(argv[3], "-n") == 0) {
            printf("N2");
            n_arg++;
            nS = argv[4];
            N = atoi(argv[4]);
            n_arg++;
        }
    
    if (argc > 2 && strcmp(argv[1], "-m") == 0) {
        printf("M1");
        n_arg++;
        mS = argv[2];
        M = atoi(argv[2]);
        n_arg++;
        } else if (argc > 4 && strcmp(argv[3], "-m") == 0) {
            printf("M2");
            n_arg++;
            mS = argv[4];
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
    
    if (fileTotal < N) {
        N = fileTotal;
    }

    //argumets
    int nArguments=3+fileTotal;
    char str[10];
    sprintf(str, "%d", fileTotal);
    
    string cmd = malloc(sizeof(char)*10000);
    strcat(cmd, "../counter/prova ");
    strcat(cmd, nS);
    strcat(cmd, " ");
    strcat(cmd, mS);
    strcat(cmd, " ");
    strcat(cmd, str);
    strcat(cmd, " ");
    for(i=0; i<fileTotal; i++){
        strcat(cmd, files[i]);
        strcat(cmd, " ");
    }

    printf("%s\n", cmd);
    system(cmd);    
    return 0;
}