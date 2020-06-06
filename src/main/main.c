#include "main_functions.h"

int main() {
    if (!DEBUG) {
        int i;
        for (i = 1; i <= 31; i++) {
            signal(i, ignoreSignal);
        }
    }

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

        printf("What do you want to do now?\n");
        printf("1. Add files to analyze\n");
        printf("2. Get file analytics\n");
        printf("Enter Your choice: ");
        fgets(mainChoice, MAX_INPUT_LENGHT, stdin);  // Input main choice
        removeNewline(mainChoice);

        if (!strcmp(mainChoice, "1")) {  // If choice equals 1
            printf("Enter space-separeted paths: ");
            fgets(choice11, MAX_INPUT_LENGHT, stdin);
            removeNewline(choice11);

            do {
                printf("Quanti m: ");
                fgets(choice12, MAX_INPUT_LENGHT, stdin);
                removeNewline(choice12);
            } while(!stringIsInt(choice12));

            do {
                printf("Quanti n: ");
                fgets(choice13, MAX_INPUT_LENGHT, stdin);
                removeNewline(choice13);
            } while(!stringIsInt(choice13));

            splitAndSendPaths(choice11, choice12, choice13);
        } else if (!strcmp(mainChoice, "2")) {  // If choice equals 2
            getAnalytics();
        } else {
            printf("This is not a valid choice, type one of the following:\n");
        }
    }
    return 0;
}
