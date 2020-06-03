#include "main.h"

void get_analytics() {
    printf("This is the table with all the analytics: TODO\n");
}

/**
 * Print error from runProgram() output function
 */
void printError(int r) {
    // TODO: remove printing when the program runs correctly
    // if (r == 0)
    //     printf("Program tarted\n");
    if (r == 1)
        printf("Error in forking\n");  // TODO: do something about not being able to fork
    else if (r == 2)
        printf("File does not exist, building it\n");
    else if (r == 3)
        printf("File is not executable, rebuilding it\n");  // TODO: check for file integrity using an hash function
    else if (r == 4)
        printf("File is a directory, rebuilding\n");
    else if (r == 5)
        printf("File is soft link, rebuilding\n");
}

int main() {
    int r;
    // Check if report is executable
    r = executableChecks("/root/bin/report");
    if (r == 2 || r == 3 || r == 4 || r == 5)
        system("cd && make clean && make build");
    // Check if analyzer is executable
    r = executableChecks("/root/bin/analyzer");
    if (r == 2 || r == 3 || r == 4 || r == 5)
        system("cd && make clean && make build");

    // char *processPath = getSelfProcessPath();
    // TODO: free(processPath)
    // printf("Process path: %s\n", processPath);
    /**  WHAT TODO NEXT
     * Now split the path of the executable /root/bin/main in /root/bin/ and add analyzer to
     * get /root/bin/analyzer, than check the file correctness.
     * 
     * Than make function GetRunningExecutablePath()
     */

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
            get_analytics();
        } else {
            printf("This is not a valid choice, type one of the following:\n");
        }
    }
    return 0;
}
