#include "main_functions.h"

char report_path[] = "/root/bin/report";
char analyzer_path[] = "/root/bin/analyzer";

/**
 * Create tail of the list
 */
node createNode() {
    node temp;
    temp = (node)malloc(sizeof(struct LinkedList));
    temp->next = NULL;
    return temp;
}

/**
 * Add node to given head of node structure
 */
node addNode(node head, char *path) {
    removeNewline(path);
    FILE *fp;
    char commandReadlink[PATH_MAX] = "";
    strcat(commandReadlink, "readlink -f ");
    strcat(commandReadlink, path);
    fp = popen(commandReadlink, "r");  // Open pointer to command output
    if (fp == NULL) {
        if (SHOW_WARNING)
            printf("Warning: Failed to run command, returning\n");
        return head;
    }

    char p[PATH_MAX];  // Max lenght of the path
    printf("Given head: %s\n", head->str);
    fgets(p, sizeof(p), fp);  // Get only first line

    removeNewline(p);
    pclose(fp);  // Close pointer to command output
    node temp = createNode();
    temp->str = p;  // Pointer to new string
    printf("Before head: %s\n", head->str);
    if (head == NULL) {
        printf("Is null\n");
        head = temp;
    } else {  // Go to head
        printf("Is not null\n");
        while (head->next != NULL) {
            head->next = head;
        }
        head->next = temp;
    }
    printf("After head: %s, %s\n", head->str, head->next->str);
    return head->next;
}

/**
 * Given a path of a directory it returns all files inside it.
 * Used command: find path -type f
 */
node listFiles(node head, char *path) {
    char *command = malloc(PATH_MAX);
    command = concat("find ", path, " -type f");
    FILE *fp;
    fp = popen(command, "r");  // Open pointer to command output
    free(command);
    if (fp == NULL) {
        if (SHOW_WARNING)
            printf("Warning: Failed to run command, returning\n");
        return head;
    }

    char p[PATH_MAX];  // Max lenght of the path
    while (fgets(p, sizeof(p), fp) != NULL) {
        head = addNode(head, p);
    }

    pclose(fp);   // Close pointer to command output
    return head;  // First node is NULL, so return second
}

/**
 * Concatenate 3 strings in 1.
 */
char *concat(const char *s1, const char *s2, const char *s3) {
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);  // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

/**
 * Removes new line character from a given string
 * e.g. "/root/bin/main\n" -> "/root/bin/main"
 * 
 * Credits: https://stackoverflow.com/a/28462221/7924557
 */
void removeNewline(char *string) {
    // printf("Before string: %s\n", string);
    string[strcspn(string, "\n")] = 0;
    // printf("After string: %s\n", string);
}

/**
 * Input a string of paths, separate on the spaces, and run concatenated
 * with the file path specified
 * 
 * Credits: https://stackoverflow.com/a/28462221/7924557
 */
void splitAndSendPaths(char *string, char *n, char *m) {
    if (string[MAX_INPUT_LENGHT - 2] != '\0' && SHOW_WARNING)
        printf("Warning: hai superato il limite di %d caratteri, l'ulima path potrebbe non essere considerata\n", MAX_INPUT_LENGHT);
    char arguments[MAX_ARG_STRLEN * 4];  // *4 in case the path is "a" there is " -c " chars
    strcat(arguments, analyzer_path);
    strcat(arguments, " -n ");
    strcat(arguments, n);
    strcat(arguments, " -m ");
    strcat(arguments, m);

    char *singlePath;                  // Contains the splited path, e.g. /root/test/file.txt
    singlePath = strtok(string, " ");  // Split in space
    while (singlePath != NULL) {
        strcat(arguments, " -c ");
        strcat(arguments, singlePath);
        singlePath = strtok(NULL, " ");
    }
    system(arguments);
    // char *a[] = {analyzer_path, arguments};
    // runProgramAndWait(a);
}

/**
 * Returns the path of the executable I'm running.
 * e.g. /root/bin/concatenated
 * e.g. /root/bin/main
 */
char *getSelfProcessPath() {
    char *buff = malloc(PATH_MAX);
    ssize_t len = readlink("/proc/self/exe", buff, PATH_MAX - 1);
    if (len != -1) {
        buff[len] = '\0';
        return buff;
    }
    return "";  // Error
    // TODO: handle error condition
}

void getAnalytics() {
    if (SHOW_WARNING)
        printf("This is the table with all the analytics: TODO\n");
}

/**
 * This function runs the program specified on path, in a completely separated process.
 * The program is checked for existance
 */
int runProgram(char **path) {
    executableChecks(path[0]);
    int pid = fork();
    if (pid == -1)  // Error in forking
        return 1;
    else if (pid == 0)  // Child section
        execvp(path[0], path);
    return 0;
}

/**
 * This function runs the program specified on path and wait for it to end.
 */
int runProgramAndWait(char **path) {
    // Check for executable errors
    executableChecks(path[0]);
    int pid = fork();

    if (pid == -1) {  // Error in forking
        return 1;
    } else if (pid == 0) {  // Child section
        printf("path: %s\n", path[0]);
        execvp(path[0], path);
    } else {
        if (SHOW_WARNING)
            printf("I'm the parent %d, waiting for child: %d\n", getpid(), pid);
        wait(NULL);
        if (SHOW_WARNING)
            printf("Waited, now ending\n");
    }
    return 0;
}

/**
 * Give a path returns the folder where the file is contained.
 * e.g. /root/bin/main -> /root/bin/main
 * e.g. /root/bin      -> /root/
 */
char *baseName(char *path) {
    char *tokens[PATH_MAX];  // TODO: replace PATH_MAX
    char *token = strtok(path, "/");
    int i = 0;
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, "/");
    }

    i = 0;
    // TODO: replace MAX_INPUT_LENGHT with the real max
    char *outString = malloc(MAX_INPUT_LENGHT);

    // Adds slash in "/root/bin/"
    //                ^
    strcat(outString, "/");
    while (tokens[i + 1] != NULL) {
        strcat(outString, tokens[i++]);
        // Adds slash in "/root/bin/"
        //                     ^   ^
        strcat(outString, "/");
    }
    return outString;
}

/**
 * Get where current program is executed and make file
 */
void makeFiles(char *processPath) {
    char command[PATH_MAX + 4 + 29];
    strcat(command, "cd ");
    strcat(command, baseName(processPath));
    strcat(command, " && make clean && make build");
    system(command);
}

/**
 * Get path and file, merge and check for integrity, if not rebuild.
 * e.g.
 * thisBaseName = "/root/bin/"
 */
int checkIntegrity(char *file, char *processPath) {
    char *concatenatedPath = malloc(PATH_MAX);
    concatenatedPath = concatPaths(processPath, file);
    int r = executableChecks(concatenatedPath);
    if (r == 2 || r == 3 || r == 4 || r == 5)
        return 1;  // Rebuild
    free(concatenatedPath);
    return 0;  // Do not rebuild
}

/**
 * Check if is an executable file.
 */
int executableChecks(char *path) {
    if (!pathIsFile(path))
        return 2;
    else if (!pathIsExecutable(path))
        return 3;
    else if (pathIsFolder(path))
        return 4;
    else if (pathIsLink(path))
        return 5;
    return 0;
}

/**
 * Print error from runProgram() output function
 */
void printError(int errNumber) {
    // TODO: remove printing when the program runs correctly
    // if (r == 0)
    //     printf("Program tarted\n");
    if (SHOW_WARNING) {
        printf("Warning: ");
        if (errNumber == 1)
            printf("Not able to fork, trying again...\n");
        else if (errNumber == 2)
            printf("File does not exist, building it\n");
        else if (errNumber == 3)
            printf("File is not executable, rebuilding it\n");
        else if (errNumber == 4)
            printf("File is a directory, rebuilding\n");
        else if (errNumber == 5)
            printf("File is soft link, rebuilding\n");
    }
}

/**
 * Returns if path is a file and the file is executable
 * 
 * Credits: https://stackoverflow.com/a/13098645/7924557
 */
int pathIsExecutable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR);
}

/**
 * Returns if path is a file
 * 
 * Credits: https://stackoverflow.com/a/230070/7924557
 */
int pathIsFile(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0);
}

/**
 * Returns if path is a folder
 * 
 * Credits: https://stackoverflow.com/a/4553053/7924557
 */
int pathIsFolder(char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0)
        return 0;
    return S_ISDIR(sb.st_mode);
}

/**
 * Returns if path is a link
 * 
 * Credits: https://stackoverflow.com/a/3985085/7924557
 */
int pathIsLink(char *path) {
    struct stat sb;
    int x;
    x = lstat(path, &sb);
    return S_ISLNK(sb.st_mode);
}

/**
 * Concatenate folder with the file:
 * e.g. dir = "/root/bin", path = "concatenated" -> "/root/bin/concatenated"
 */
char *concatPaths(char *dir, char *file) {
    char *retPath = malloc(PATH_MAX);
    retPath = strcat(retPath, dir);
    retPath = strcat(retPath, file);
    return retPath;
}

/**
 * Ignore signal and print warning.
 * 
 * Credits: https://stackoverflow.com/a/25526951/7924557
 */
void ignoreSignal(int signal) {
    if (SHOW_WARNING)
        printf("\nWarining: ignoring signal\n");
}

/**
 * Check if string is an integer or not
 */
int stringIsInt(char *string) {
    char *newString;
    long number = strtol(string, &newString, 10);  // Base 10

    if (*newString != '\0' || newString == string) {
        printf("Scrivi un numero\n");
    } else {
        return TRUE;
    }
    return FALSE;
}