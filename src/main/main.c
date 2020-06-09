#include "main_functions.h"

char clean_path[] = "/root/bin/clean";

int main() {
    if (!DEBUG) {
        int i;
        for (i = 1; i <= 31; i++) {
            signal(i, ignoreSignal);
        }
    }
    signal(SIGUSR2, ignoreSignal);
    signal(SIGUSR1, runReport);

    char *fullProcessPath = getSelfProcessPath();
    char *processPath = baseName(fullProcessPath);

    if (SHOW_WARNING && strcmp(processPath, "/root/bin/"))
        printf("Warning: /root/bin/ not detected, new folder is: %s\n", processPath);

    if (checkIntegrity("analyzer", processPath) || checkIntegrity("report", processPath)) {
        printf("Some files were modified, rebuilding...\n");
        makeFiles(processPath);  // Reduild
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
        memset(choice2, 0, sizeof choice2);

        printf("Cosa vuoi fare ora?\n");
        printf("1. Aggiungi file da analizzare\n");
        printf("2. Visualizza dati analizzati\n");
        printf("3. Chiudi il programma\n");
        printf("Inserisci la tua scelta: ");
        fgets(mainChoice, MAX_INPUT_LENGHT, stdin);  // Input main choice
        removeNewline(mainChoice);

        if (!strcmp(mainChoice, "1")) {  // If choice equals 1
            printf("Inserisci il percorso delle cartelle che vuoi analizzare: ");
            fgets(choice11, MAX_INPUT_LENGHT, stdin);
            removeNewline(choice11);

            do {
                printf("Quanti il numero di processi P: ");
                fgets(choice12, MAX_INPUT_LENGHT, stdin);
                removeNewline(choice12);
            } while (!stringIsInt(choice12));

            do {
                printf("Quanti il numero di processi Q: ");
                fgets(choice13, MAX_INPUT_LENGHT, stdin);
                removeNewline(choice13);
            } while (!stringIsInt(choice13));

            splitAndSendPaths(choice11, choice12, choice13);
        } else if (!strcmp(mainChoice, "2")) {  // If choice equals 2
            getAnalytics();
        } else if (!strcmp(mainChoice, "3")) {  // If choice equals 2
            char *a[] = {clean_path};
            runProgramAsProcess(a);
            return 0;
        } else {
            printf("Non hai fatto una scelta corretta, inserisci il numero di una tra una delle seguenti funzioni:\n");
        }
    }
    return 0;
}
