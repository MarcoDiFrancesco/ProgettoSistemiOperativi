#include "main.h"

char *report_path[] = {"/root/bin/report", NULL};
char *analyzer_path[] = {"/root/bin/analyzer", NULL};

/**
 * This function runs the program specified on path, in a completely separated process.
 * The program is checked for existance
 */
int runProgram(char **path) {
    if (!pathIsFile(path[0]))
        return 2;
    else if (!pathIsExecutable(path[0]))
        return 3;
    else if (pathIsFolder(path[0]))
        return 4;
    else if (pathIsLink(path[0]))
        return 5;

    int pid = fork();

    if (pid == -1) {  // Error in forking
        return 1;
    } else if (pid == 0) {  // Child section
        execvp(path[0], path);
    }
    return 0;
}

void get_analytics() {
    printf("This is the table with all the analytics: TODO\n");
}

int main() {
    int r = 0;
    // r = runProgram(analyzer_path);
    if (r == 0)
        printf("Analyzer started\n");
    else if (r == 1)
        printf("Error in forking\n");  // TODO: do something about not being able to fork
    else if (r == 2)
        printf("File does not exist, building it\n");
    else if (r == 3)
        printf("File is not executable, rebuilding it\n");  // TODO: check for file integrity using an hash function
    else if (r == 4)
        printf("File is a directory, rebuilding\n");
    else if (r == 5)
        printf("File is soft link, rebuilding\n");

    // TODO: get path where the program is run and replace the absolute path with ./
    if (r == 2 || r == 3 || r == 4 || r == 5)
        system("cd && make clean && make build");

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
            sendPaths(s);
        } else if (!strcmp(choice, "2")) {  // If choice equals 2
            get_analytics();
        } else {
            printf("This is not a valid choice, type one of the following:\n");
        }
    }
    return 0;
}
