#include "analyzer.h"

int N = 3;
int M = 4;
int n_arg = 1;
string nS = "3";
string mS = "4";

int main(int argc, string argv[]) {
    signal(SIGUSR2, ignoreSignal);
    signal(SIGUSR1, sendSignal);
    BOOL flagMain = FALSE;
    if (strcmp(argv[argc - 1], "-a")) {
        //flag main setted
        flagMain = TRUE;
        printf("Flag set\n");
    }
    //scrivo qui controllo degli argomenti per il C
    if (argc > 2 && strcmp(argv[1], "-n") == 0 && atoi(argv[2]) > 0 && atoi(argv[2]) < MAXQ) {
        //printf("N1");
        n_arg++;
        nS = argv[2];
        N = atoi(argv[2]);
        n_arg++;
    } else if (argc > 4 && strcmp(argv[3], "-n") == 0 && atoi(argv[4]) > 0 && atoi(argv[4])  < MAXQ) {
        //printf("N2");
        n_arg++;
        nS = argv[4];
        N = atoi(argv[4]);
        n_arg++;
    }

    if (argc > 2 && strcmp(argv[1], "-m") == 0 && atoi(argv[2]) > 0 && atoi(argv[2]) < MAXQ) {
        //printf("M1");
        n_arg++;
        mS = argv[2];
        M = atoi(argv[2]);
        n_arg++;
    } else if (argc > 4 && strcmp(argv[3], "-m") == 0 && atoi(argv[4]) > 0 && atoi(argv[4]) < MAXQ) {
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
        if (strcmp(arg[a], "-c") == 0) {
            if (argc > a + 1 + total_elements_in_folder) {
                int elements_in_folder = 0;
                if (containsDot(arg[a + 1]) == FALSE) {
                    node l = listFiles(arg[a + 1]);
                    node t = l;
                    //elements_in_folder = 0;
                    while (t != NULL) {
                        elements_in_folder++;
                        t = t->next;
                    }
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
    //controllo sul nome dei file passati come argomento
    BOOL filesOk[argc - n_arg];
    int fileErrati = 0;
    for (i = n_arg; i < argc; i++) {
        int lunghezza_nome = strlen(arg[i]);
        if (isTxt(arg[i], lunghezza_nome) == TRUE || isC(arg[i], lunghezza_nome) == TRUE ||
            isCpp(arg[i], lunghezza_nome) == TRUE || isPy(arg[i], lunghezza_nome) == TRUE ||
            isJava(arg[i], lunghezza_nome) == TRUE) {
            filesOk[i] = TRUE;
        } else {
            filesOk[i] = FALSE;
            fileErrati++;
        }
    }
    // Controllo sull'esistenza dei file passati
    int fileInesistenti = 0;
    for (i = n_arg; i < argc; ++i) {
        if (filesOk[i]) {
            if (access(arg[i], F_OK) == -1) {
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
    string *files = malloc(fileTotal * sizeof(char *));
    int count = 0;
    for (i = n_arg; i < argc; ++i) {
        if (filesOk[i] == TRUE) {
            count++;
        }
    }

    int next = 0;
    for (i = n_arg; i < argc; i++) {
        if (filesOk[i] == TRUE) {
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

    //system("/root/bin/cleanBuffer");
    clean(ProjectId, PathName);
    clean(3, PathName2);
    clean(6, "/root/src/main/main.c");

    string cmdList[fileTotal + 4];
    int cmdListCount = 1;
    string cmd = malloc(sizeof(char) * fileTotal * 1000 + 100);
    cmdList[0] = "/root/bin/counter";
    cmdList[cmdListCount++] = nS;
    cmdList[cmdListCount++] = mS;
    cmdList[cmdListCount++] = str;
    for (i = 0; i < fileTotal; i++) {
        cmdList[cmdListCount++] = files[i];
    }
    cmdList[cmdListCount++] = NULL;

    pid_t CounterPid;
    CounterPid = fork();
    if (CounterPid == 0) {
        if (flagMain == TRUE) {
            cmdList[--cmdListCount] = "-a";
            cmdList[cmdListCount++] = NULL;
        }
        execv(cmdList[0], cmdList);
    } else {
        //potremmo inserire una msg
        string back = recConfirm(6, "/root/src/main/main.c");
        while (strcmp(back, "r") != 0) {
            back = recConfirm(6, "/root/src/main/main.c");
        }
        fflush(stdout);
        printf("\nvuoi inserire roba nuova testina di cazz0? [y/n]\n");
        char input;
        fflush(stdin);
        scanf(" %c", &input);
        while (input != 'n' && input != 'y') {
            puts("Inserisci solamente [y/n]");
            fflush(stdin);
            scanf(" %c", &input);
        }
        if (input == 'n') {
            sendConfirm("n", 3, PathName2);
        } else if (input == 'y') {
            sendConfirm("y", 3, PathName2);
            string pipemsg = malloc(MAXLEN);
            string *allChanges = malloc(0);
            int contchanges = 0;
            do {
                allChanges = realloc(allChanges, (contchanges + 1) * sizeof(string));
                allChanges[contchanges] = malloc(MAXLEN);
                strcpy(pipemsg, addThingsToCounter());

                if (strcmp(pipemsg, "\n") != 0) {
                    strcpy(allChanges[contchanges], pipemsg);
                    contchanges++;
                }
            } while (strcmp(pipemsg, "X") != 0);

            for (i = 0; i < contchanges; i++) {
                printf(">"); puts(allChanges[i]);
            }

            string lastN = malloc(10);
            strcpy(lastN, " ");
            string lastM = malloc(10);
            strcpy(lastM, " ");
            for (i = contchanges - 1; i >= 0; i--) {
                if (strcmp(lastN, " ") == 0 && allChanges[i][1] == 'n') {
                    strcpy(lastN, allChanges[i]);
                }
                if (strcmp(lastM, " ") == 0 && allChanges[i][1] == 'm') {
                    strcpy(lastM, allChanges[i]);
                }
            }
            string totalPipeMessage = malloc(sizeof(char) * contchanges * MAXLEN + 100);
            strcpy(totalPipeMessage, lastN);
            strcat(totalPipeMessage, " ");
            strcat(totalPipeMessage, lastM);
            for (i = 0; i < contchanges - 1; i++) {
                if (allChanges[i][1] != 'n' && allChanges[i][1] != 'm') {
                    strcat(totalPipeMessage, " ");
                    strcat(totalPipeMessage, allChanges[i]);
                }
            }
            printf("> new message: %s", totalPipeMessage);
        }
        waitpid(CounterPid, NULL, 0);
    }
    free(cmd);
    free(files);
    printf("Analyzer ended\n");
    return 0;
}