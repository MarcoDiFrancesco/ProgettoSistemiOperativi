#include "../main/main_functions.h"
#include "analyzer.h"

int N = 3;
int M = 4;
int n_arg = 1;
char *nProcesses = "3";
char *mProcesses = "4";

int main(int argc, char *argv[]) {
    getArgument("-n", argc, argv, &n_arg, &nProcesses, &N);
    getArgument("-m", argc, argv, &n_arg, &mProcesses, &M);

    int i;
    char **args = malloc(sizeof(string) * argc);
    for (i = 0; i < argc; i++) {
        args[i] = argv[i];
    }

    node filesList = createNode();  // Tail of the list
    node head = filesList;
    int nArg;
    int stupido = 0;
    for (nArg = n_arg; nArg < argc; nArg++) {
        printf("n element: %d, contains: %s\n", nArg, args[nArg]);
        if (pathIsFolder(args[nArg]))
            head = listFiles(head, args[nArg]);
        else if (pathIsFile(args[nArg]))
            head = addNode(head, args[nArg]);
        else
            printf("Warning: File non testuale: %s\n", args[nArg]);

        // //printf(" %s flag detected - (%d) ",args[nArg], nArg);
        // //printf("argc=%d nArg+1+stupido=%d\n", argc, nArg+1+stupido);
        // if (argc > nArg + 1 + stupido) {
        //     int elements_in_folder = 0;
        //     //printf(" - next exists - ");
        //     filesList = listFiles(args[nArg + 1]);
        //     node tmp = filesList;
        //     //elements_in_folder = 0;
        //     while (tmp != NULL) {
        //         elements_in_folder++;
        //         tmp = tmp->next;
        //     }
        //     //printf("\nelementi trovati nella cartella: %d\n", elements_in_folder);

        //     args = realloc(args, sizeof(string) * (argc + elements_in_folder));
        //     i = argc;
        //     while (filesList != NULL) {
        //         args[i] = filesList->str;
        //         i++;
        //         filesList = filesList->next;
        //     }
        //     argc += elements_in_folder;
        //     stupido += elements_in_folder;
        // } else {
        //     n_arg++;
        //     printf("Second else\n");
        // }
    }
    node tmp = filesList;
    while (tmp != NULL) {
        printf("File: %s\n", tmp->str);
        tmp = tmp->next;
    }

    for (nArg = n_arg; nArg < argc; nArg++) {
        printf("file %d = %s\n", argc - nArg, args[nArg]);
    }

    //printf("\n(numero di argomenti inseriti prima dei file %d) \nN: %d\nM: %d\n", n_arg - 1, N, M);

    //controllo sul nome dei file passati come argomento
    BOOL filesOk[argc - n_arg];
    int fileErrati = 0;
    printf("nome corretto: ");
    for (i = n_arg; i < argc; i++) {
        int lunghezza_nome = strlen(args[i]);
        if (isTxt(args[i], lunghezza_nome) == TRUE || isC(args[i], lunghezza_nome) == TRUE ||
            isCpp(args[i], lunghezza_nome) == TRUE || isPy(args[i], lunghezza_nome) == TRUE ||
            isJava(args[i], lunghezza_nome) == TRUE) {
            filesOk[i] = TRUE;
            printf("> %s OK (%d)\n", args[i], i);
        } else {
            filesOk[i] = FALSE;
            fileErrati++;
            printf("> %s NO (%d)\n", args[i], i);
        }
    }

    // Controllo sull'esistenza dei file passati
    int fileInesistenti = 0;
    puts("\n\nControllo filesList'esistenza dei file validi\n");
    for (i = n_arg; i < argc; ++i) {
        if (filesOk[i]) {
            if (access(args[i], F_OK) == -1) {
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
            if (i == argc - 1) {
                //printf("DIO PORCO BOIA MAIALE ");
            }
            count++;
            //printf("%d ", count);
        }
    }

    int next = 0;

    /*for (i = n_arg; i < argc; i++) {
        //printf("indice di avanzamento %d\n", i);
        if (filesOk[i] == TRUE) {
            files[next] = args[i];
            //Printf("---IL ROMPICAZZO %s (%d)\n", files[next], next);
            next++;
        } 
    }*/
    for (i = n_arg; i < argc; i++) {
        //printf("indice di avanzamento %d\n", i);
        if (filesOk[i] == TRUE) {
            /*if (i == argc - 1) {
                printf("DIO PORCO BOIA MAIALE ");
            }*/
            //printf("---IL ROMPICAZZO %s ()\n", args[i]);
            files[next] = args[i];
            next++;
        } /*else if (fileTotal % 40 == 0  && i == argc - 1) {
            //printf("---IL ROMPICAZZO %s ()\n", args[i]);
            files[next] = args[i];
        } else {
            //puts("questo è stupido\n");
        } */
    }
    /*
    for (i = n_arg; i < argc; i++) {
        //printf("indice di avanzamento %d\n", i);
        if (filesOk[i] == TRUE) {
            //printf("---IL ROMPICAZZO %s ()\n", args[i]);
            files[next] = args[i];
            next++;
        } else if (filesOk) {
            //printf("---IL ROMPICAZZO %s ()\n", args[i]);
            files[next] = args[i];
        } else {
            //puts("questo è stupido\n");
        }
        
    }*/

    if (fileTotal < N) {
        N = fileTotal;
        nProcesses = malloc(sizeof(MAXLEN));
        sprintf(nProcesses, "%d", N);
    }

    //argumets
    int nArguments = 3 + fileTotal;
    char str[100];
    sprintf(str, "%d", fileTotal);

    printf("\nfileTotal=%d\n", fileTotal);
    string cmd = malloc(sizeof(char) * fileTotal * 1000 + 100);
    strcat(cmd, "./bin/counter ");
    strcat(cmd, nProcesses);
    strcat(cmd, " ");
    strcat(cmd, mProcesses);
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