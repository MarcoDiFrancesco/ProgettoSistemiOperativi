#include "analyzer.h"

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

    int a, i;
    printf("\nfiles inseriti =  %d", argc - n_arg);
    for (a = n_arg; a<argc; a++) {
        if(argv[a] == "-c" && a+1<argc){
            node l = listFiles(argv[a+1]);
            int elements_in_folder = 0;
            while(l->next != NULL){
                elements_in_folder++;
                l = l->next;
            }

            string tmp[argc + elements_in_folder -2];
            for(i=0; i<argc; i++){
                if(i != a && i != a+1)
                    tmp[i] = argv[i];
            }

            argv = tmp;

            while(l->next != NULL){
                argv[i] = l->str;
                i++;
                l = l->next;
            }

            printf("\nnuovi argomenti dopo aver analizzato la cartella");
            for(i=0; i<argc+elements_in_folder-2; i++){
                printf("\n> %s", argv[i]);
            }
            printf("\n---\n");
        }
        printf("\nfile %d = %s", argc - a, argv[a]);
    }

    printf("\n(numero di argomenti inseriti prima dei file %d) \nN: %d\nM: %d\n", 
               n_arg - 1, N, M);

    //controllo sul nome dei file passati come argomento
    BOOL filesOk[argc - n_arg];
    int fileErrati = 0;
    printf("nome corretto: ");
    for (i = n_arg; i < argc; i++) {
        int lunghezza_nome = strlen(argv[i]);
        if(isTxt(argv[i], lunghezza_nome) || isC(argv[i], lunghezza_nome) ||
           isCpp(argv[i], lunghezza_nome) || isPy(argv[i], lunghezza_nome) ||
           isJava(argv[i], lunghezza_nome)) {
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
        nS=malloc(sizeof(MAXLEN));
        sprintf(nS, "%d", N);
    }

    //argumets
    int nArguments=3+fileTotal;
    char str[10];
    sprintf(str, "%d", fileTotal);
    
    string cmd = malloc(sizeof(char)*10000);
    strcat(cmd, "counter/main ");
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

    free(cmd);
    return 0;
}