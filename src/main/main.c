#include "main_functions.h"

#ifndef BINARY_PATHS
#define BINARY_PATHS
char analyzer_path[] = "/root/bin/analyzer";
char counter_path[] = "/root/bin/counter";
char clean_path[] = "/root/bin/cleanBuffer";
char main_path[] = "/root/bin/main";
char report_path[] = "/root/bin/report";
#endif

int main() {
    if (!DEBUG) {
        int i;
        for (i = 1; i <= 31; i++) {
            signal(i, ignoreSignal);
        }
    }
    signal(SIGUSR2, ignoreSignal);
    signal(SIGUSR1, runReport);

    if (checkIntegrity(analyzer_path) ||
        checkIntegrity(counter_path) ||
        checkIntegrity(clean_path) ||
        checkIntegrity(main_path) ||
        checkIntegrity(report_path)) {
        printf("Some files were modified, rebuilding...\n");
        makeFiles("/root/");  // Reduild
    }

    char mainChoice[MAX_INPUT_LENGHT];
    char choice11[MAX_INPUT_LENGHT];
    char choice12[MAX_INPUT_LENGHT];
    char choice13[MAX_INPUT_LENGHT];
    char choice2[MAX_INPUT_LENGHT];

    while (1) {
        // Empty the arrays
        memset(mainChoice, 0, sizeof mainChoice);
        memset(choice11, 0, sizeof choice11);
        memset(choice12, 0, sizeof choice12);
        memset(choice13, 0, sizeof choice13);

        printf("Cosa vuoi fare ora?\n");
        printf("1. Aggiungi file da analizzare\n");
        printf("2. Chiudi il programma\n");
        printf("Inserisci la tua scelta: ");
        fgets(mainChoice, MAX_INPUT_LENGHT, stdin);
        removeNewline(mainChoice);

        if (!strcmp(mainChoice, "1")) {
            do {
                printf("Inserisci percorsi di cartelle che vuoi analizzare separati da spazio, in caso l'indirizzo non sia valido gli indirizzi verranno ignorati: ");
                fgets(choice11, MAX_INPUT_LENGHT, stdin);
                removeNewline(choice11);
            } while (choice11[0] == '\0');

            do {
                printf("Quanti il numero di processi P [default 3]: ");
                fgets(choice12, MAX_INPUT_LENGHT, stdin);
                removeNewline(choice12);
            } while (!choice12[0] == '\0' && !stringIsInt(choice12));

            do {
                printf("Quanti il numero di processi Q [default 4]: ");
                fgets(choice13, MAX_INPUT_LENGHT, stdin);
                removeNewline(choice13);
            } while (!choice13[0] == '\0' && !stringIsInt(choice13));

            splitAndSendPaths(choice11, choice12, choice13);
        } else if (!strcmp(mainChoice, "2")) {
            clean(ProjectId, PathName);
            clean(3, PathName2);
            clean(6, "/root/src/main/main.c");
            clean(7, "/root/src/main/main_functions.h");
            return 0;
        } else {
            printf("Non hai fatto una scelta corretta, inserisci il numero di una tra una delle seguenti funzioni:\n");
        }
    }
    return 0;
}
