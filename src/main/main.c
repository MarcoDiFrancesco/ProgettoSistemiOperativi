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

    char mainChoice[MAX_INPUT_LENGHT], choice1[MAX_INPUT_LENGHT], choice2[MAX_INPUT_LENGHT];
    while (1) {
        // Empty the arrays
        memset(mainChoice, 0, sizeof mainChoice);
        memset(choice1, 0, sizeof choice1);
        memset(choice2, 0, sizeof choice2);

        printf("What do you want to do?\n");
        printf("1. Add files to analyze\n");
        printf("2. Get file analytics\n");
        printf("Enter Your choice: ");
        fgets(mainChoice, MAX_INPUT_LENGHT, stdin); // Input main choice
        removeNewline(mainChoice);

        if (!strcmp(mainChoice, "1")) {  // If choice equals 1
            printf("Enter space-separeted paths: ");
            fgets(choice1, MAX_INPUT_LENGHT, stdin);
            removeNewline(choice1);
            splitAndSendPaths(choice1);
        } else if (!strcmp(mainChoice, "2")) {  // If choice equals 2
            getAnalytics();
        } else {
            printf("This is not a valid choice, type one of the following:\n");
        }
    }
    return 0;
}
