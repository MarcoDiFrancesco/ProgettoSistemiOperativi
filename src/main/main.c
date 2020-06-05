#include "main.h"

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

    char s[MAX_INPUT_LENGHT], choice[MAX_INPUT_LENGHT];
    while (1) {
        // Empty the array
        memset(s, 0, sizeof s);
        memset(choice, 0, sizeof choice);

        printf("1. Add files to analyze\n");
        printf("2. Get file analytics\n");
        printf("Enter Your choice: ");
        // Input choice
        fgets(choice, MAX_INPUT_LENGHT, stdin);
        removeNewline(choice);

        if (!strcmp(choice, "1")) {  // If choice equals 1
            printf("Enter space-separeted paths: ");
            fgets(s, MAX_INPUT_LENGHT, stdin);
            removeNewline(s);
            splitAndSendPaths(s);
        } else if (!strcmp(choice, "2")) {  // If choice equals 2
            getAnalytics();
        } else {
            printf("This is not a valid choice, type one of the following:\n");
        }
    }
    return 0;
}
