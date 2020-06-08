#include "analyzer.h"

int N = 3;
int M = 4;
int n_arg = 1;
string nS = "3";
string mS = "4";

int main(int argc, string argv[]) {
    //scrivo qui controllo degli argomenti per il C
    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        //printf("N1");
        n_arg++;
        nS = argv[2];
        N = atoi(argv[2]);
        n_arg++;
    } else if (argc > 4 && strcmp(argv[3], "-n") == 0) {
        //printf("N2");
        n_arg++;
        nS = argv[4];
        N = atoi(argv[4]);
        n_arg++;
    }

    if (argc > 2 && strcmp(argv[1], "-m") == 0) {
        //printf("M1");
        n_arg++;
        mS = argv[2];
        M = atoi(argv[2]);
        n_arg++;
    } else if (argc > 4 && strcmp(argv[3], "-m") == 0) {
        //printf("M2");
        n_arg++;
        mS = argv[4];
        M = atoi(argv[4]);
        n_arg++;
    }

    int a, i;
    //printf("\n (%d) files inseriti =  %d",argc,  argc - n_arg);

    string *arg = malloc(sizeof(string) * argc);
    for (i = 0; i < argc; i++) {
        arg[i] = argv[i];
    }

    int total_elements_in_folder = 0;
    for (a = n_arg; a < argc; a++) {
        //printf("\nelemento: %d  contene: %s", a, arg[a]);
        if (strcmp(arg[a], "-c") == 0) {
            //printf(" %s flag detected - (%d) ",arg[a], a);
            //printf("argc=%d a+1+stupido=%d\n", argc, a+1+stupido);
            if (argc > a + 1 + total_elements_in_folder) {
                int elements_in_folder = 0;
                //printf(" - next exists - ");
                if (containsDot(arg[a + 1]) == FALSE) {
                    node l = listFiles(arg[a + 1]);
                    node t = l;
                    //elements_in_folder = 0;
                    while (t != NULL) {
                        elements_in_folder++;
                        t = t->next;
                    }
                    //printf("\nelementi trovati nella cartella: %d\n", elements_in_folder);

                    arg = realloc(arg, sizeof(string) * (argc + elements_in_folder));
                    i = argc;
                    while (l != NULL) {
                        arg[i] = l->str;
                        i++;
                        l = l->next;
                    }
                    argc += elements_in_folder;
                    total_elements_in_folder += elements_in_folder;
                }
            } else {
                n_arg++;
            }
        }
    }

    for (a = n_arg; a < argc; a++) {
        //printf("\nfile %d = %s", argc - a, arg[a]);
    }

    //printf("\n(numero di argomenti inseriti prima dei file %d) \nN: %d\nM: %d\n", n_arg - 1, N, M);

    //controllo sul nome dei file passati come argomento
    BOOL filesOk[argc - n_arg];
    int fileErrati = 0;
    //printf("nome corretto: ");
    for (i = n_arg; i < argc; i++) {
        int lunghezza_nome = strlen(arg[i]);
        if (isTxt(arg[i], lunghezza_nome) == TRUE || isC(arg[i], lunghezza_nome) == TRUE ||
            isCpp(arg[i], lunghezza_nome) == TRUE || isPy(arg[i], lunghezza_nome) == TRUE ||
            isJava(arg[i], lunghezza_nome) == TRUE) {
            filesOk[i] = TRUE;
            //printf("> %s OK (%d)\n", arg[i], i);
        } else {
            filesOk[i] = FALSE;
            fileErrati++;
            //printf("> %s NO (%d)\n", arg[i], i);
        }
    }

    // Controllo sull'esistenza dei file passati
    int fileInesistenti = 0;
    //puts("\n\nControllo l'esistenza dei file validi\n");
    for (i = n_arg; i < argc; ++i) {
        if (filesOk[i]) {
            if (access(arg[i], F_OK) == -1) {
                filesOk[i] = FALSE;
                fileInesistenti++;
            }
        }
    }

    //printf("ARGC = %d, n_arg = %d, fileErrati = %d, fileInesistenti = %d\n", argc, n_arg, fileErrati, fileInesistenti);

    int const fileTotal = argc - n_arg - fileErrati - fileInesistenti;
    if (fileTotal < 1) {
        printf("non ho ricevuto alcun file valido\n");

        exit(0);
    }

    string *files = malloc(fileTotal * sizeof(char *));
    //string files[fileTotal];
    //printf(" files OK = %d\n", fileTotal);
    int count = 0;
    for (i = n_arg; i < argc; ++i) {
        if (filesOk[i] == TRUE) {
            count++;
            //printf("%d ", count);
        }
    }

    int next = 0;
    for (i = n_arg; i < argc; i++) {
        //printf("indice di avanzamento %d\n", i);
        if (filesOk[i] == TRUE) {
            //printf("---IL ROMPICAZZO %s ()\n", arg[i]);
            files[next] = arg[i];
            next++;
        }
    }
    if (fileTotal < N) {
        N = fileTotal;
        nS = malloc(sizeof(MAXLEN));
        sprintf(nS, "%d", N);
    }

    //argumets
    int nArguments = 3 + fileTotal;
    char str[100];
    sprintf(str, "%d", fileTotal);

    printf("\nFile Totali trovati da analizzare: %d\n", fileTotal);
    string cmd = malloc(sizeof(char) * fileTotal * 1000 + 100);
    strcat(cmd, "/root/bin/counter ");
    strcat(cmd, nS);
    strcat(cmd, " ");
    strcat(cmd, mS);
    strcat(cmd, " ");
    strcat(cmd, str);
    strcat(cmd, " ");
    for (i = 0; i < fileTotal; i++) {
        strcat(cmd, files[i]);
        //free(files[i]);
        strcat(cmd, " ");
    }

    //printf("%s\n", cmd);
    system(cmd);

    free(cmd);
    free(files);

    return 0;
}