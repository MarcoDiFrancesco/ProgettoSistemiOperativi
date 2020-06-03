#include "main.h"

int main() {
    if (checkIntegrity("analyzer") || checkIntegrity("report"))
        printf("Rebuild...\n");

    // TODO: check each malloc, if it does not have memory, kill the process
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
