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

    int a, i, j;
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
    //string cmd = malloc(sizeof(char) * fileTotal * 1000 + 100);
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
            int n_new_file = 0;
            string totalPipeMessage = malloc(sizeof(char) * contchanges * MAXLEN + 100);
            strcpy(totalPipeMessage, lastN);
            strcat(totalPipeMessage, " ");
            strcat(totalPipeMessage, lastM);
            for (i = 0; i < contchanges - 1; i++) {
                if (allChanges[i][1] != 'n' && allChanges[i][1] != 'm') {
                    n_new_file++;
                    strcat(totalPipeMessage, " ");
                    strcat(totalPipeMessage, allChanges[i]);
                }
            }       
            printf("> new message: %s\n", totalPipeMessage);
            printf("strlen pipemessage: %ld\n", strlen(totalPipeMessage));
            i = 0;
            BOOL flag_n,
                 flag_m,
                 flag_f = FALSE;
            string new_n = malloc(7);
            string new_m = malloc(7);
            string *new_file_list = malloc(n_new_file);
            int n_new_file_k = 0;
            while(i < strlen(totalPipeMessage)){ //finchè non arrivo in fondo alla stringa
                //finchè non trovo un carattere '-'
                while(totalPipeMessage[i] != '-' && totalPipeMessage[i] != '\0'){
                    //printf(".%c",totalPipeMessage[i]);
                    i++;
                }
                //prendo l'elemento dopo il carattere '-'
                char flag = totalPipeMessage[++i];
                //vado dopo lo spazio
                i += 2;
                //controllo di che tipo è il flag
                string new_file;
                switch(flag){
                    case 'n':
                        flag_n = TRUE;
                        j = 0;
                        //mi salvo il contenuto finchè non raggiungo uno spazio
                        while(totalPipeMessage[i] != ' ' && totalPipeMessage[i] != '\0'){
                            new_n[j++] = totalPipeMessage[i++];
                        }
                        new_n[j] = '\0';
                        //printf("\n");
                        break;
                    case 'm':
                        flag_m = TRUE;
                        j = 0;
                        //mi salvo il contenuto finchè non raggiungo uno spazio
                        while(totalPipeMessage[i] != ' ' && totalPipeMessage[i] != '\0'){
                            new_m[j++] = totalPipeMessage[i++];
                        }
                        new_m[j] = '\0';
                        printf("\n");
                        break;
                    case 'f':
                        flag_f = TRUE;
                        new_file = malloc(MAXLEN);
                        j = 0;
                        //mi salvo il contenuto finchè non raggiungo uno spazio
                        while(totalPipeMessage[i] != ' ' && totalPipeMessage[i] != '\0'){
                            new_file[j++] = totalPipeMessage[i++];
                        }
                        new_file[j] = '\0';
                        if (access(new_file, F_OK) > -1 && fileIsValid(new_file) == TRUE){
                            new_file_list[n_new_file_k] = malloc(MAXLEN);
                            strcpy(new_file_list[n_new_file_k ++], new_file);
                        }

                        free(new_file);
                        break;
                }
            }

            string new_cmdList[fileTotal + 4 + n_new_file_k + 2];

            new_cmdList[0] = cmdList[0];

            if (flag_n == TRUE) {
                puts(new_n);
                //strcpy(new_cmdList[1], new_n);
                new_cmdList[1] = new_n;
            } else {
                //strcpy(new_cmdList[1], cmdList[1]);
                new_cmdList[1] = cmdList[1];
            }
            if (flag_m == TRUE) {
                puts(new_m);
                //strcpy(new_cmdList[2], new_m);
                new_cmdList[2] = new_m;
            } else {
                //strcpy(new_cmdList[2], cmdList[2]);
                new_cmdList[2] = cmdList[2];
            }
            
            string new_filetotal = malloc(MAXLEN); 
            sprintf(new_filetotal, "%d", fileTotal + n_new_file_k);
            //strcpy(new_cmdList[3], new_filetotal);
            new_cmdList[3] = new_filetotal;

            for(i=4; i<fileTotal+4; i++){
                //strcpy(new_cmdList[i], cmdList[i]);
                new_cmdList[i] = cmdList[i];
            }

            cmdListCount--;

            printf("numero nuovi file inseriti: %d\n", n_new_file_k );
            if(flag_f == TRUE){
                for(i = 0; i < n_new_file_k ; i++){
                    //printf("%d ", i);
                    //puts(new_file_list[i]);
                    //strcpy(new_cmdList[cmdListCount++], new_file_list[i]);
                    puts(" OOOOOOOOOOOOOO nuovo file inserito");
                    new_cmdList[cmdListCount++] = new_file_list[i];
                }
            }
            //flag che vuoi tu
            //new_cmdList[cmdListCount++] = NULL;
            if (flagMain == TRUE) {
                new_cmdList[cmdListCount++] = "-a";
                puts("areo");
            }
            new_cmdList[cmdListCount] = NULL;

            //debug
            i=0;
            while(new_cmdList[i]!=NULL){
                puts(new_cmdList[i++]);
            }
            puts(new_cmdList[cmdListCount - 1]);
            printf("I=%d\n",i);

            CounterPid=fork();
            if(CounterPid==0){
                puts("ho lanciato counter di nuovo");
                execv(new_cmdList[0], new_cmdList);
            }else{
                //cose che deve fare lanalyzer
                sendConfirm("n", 3, PathName2);
            }
        }
        waitpid(CounterPid, NULL, 0);
    }
    //free(cmd);
    free(files);
    return 0;
}