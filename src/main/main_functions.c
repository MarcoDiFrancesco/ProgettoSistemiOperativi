#include "main_functions.h"

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
node addNode(node head, char *new_str) {
    char *dest;  // Create new str
    dest = malloc(sizeof(char) * (strlen(new_str) + 1));
    strcpy(dest, new_str);  // Copy string into a new string

    node temp, p;
    temp = createNode();

    temp->str = dest;  // Pointer to new string
    if (head == NULL) {
        head = temp;
    } else {
        p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}

/**
 * Given a path of a directory it returns all files inside it.
 */
node listFiles(char *path) {
    node files_list = createNode();  // Tail of the list
    node head = files_list;
    FILE *fp;

    char *command = malloc(PATH_MAX);
    command = concat("find ", path, " -type f");
    fp = popen(command, "r");  // Open pointer to command output
    free(command);
    if (fp == NULL) {
        if (SHOW_WARNING)
            printf("Warning: Failed to run command, returning\n");
        return files_list;
    }

    char p[PATH_MAX];  // Max lenght of the path
    while (fgets(p, sizeof(p), fp) != NULL) {
        head = addNode(head, p);
    }

    pclose(fp);               // Close pointer to command output
    return files_list->next;  // First node is NULL, so return second
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
 */
void removeNewline(char *string) {
    string[strcspn(string, "\n")] = 0;
}

/**
 * Input a string of paths, separate on the spaces, and run concatenated
 * with the file path specified
 */
void splitAndSendPaths(char *files, char *n, char *m, char *folders) {
    if (files[MAX_INPUT_LENGHT - 2] != '\0' && SHOW_WARNING)
        printf("Warning: hai superato il limite di %d caratteri, l'ulima path potrebbe non essere considerata\n", MAX_INPUT_LENGHT);
    int argumentsC = 0;
    char *argumentsV[MAX_ARG_STRLEN];  // *4 in case the path is "a" there is " -c " chars
    argumentsV[argumentsC++] = "/root/bin/analyzer";

    if (n[0] != '\0') {
        argumentsV[argumentsC++] = "-n";
        argumentsV[argumentsC++] = n;
    } else {
        argumentsV[argumentsC++] = "-n";
        argumentsV[argumentsC++] = "3";
    }
    if (m[0] != '\0') {
        argumentsV[argumentsC++] = "-m";
        argumentsV[argumentsC++] = m;
    } else {
        argumentsV[argumentsC++] = "-m";
        argumentsV[argumentsC++] = "4";
    }

    char *singlePathFolders;                   // Contains the splited path, e.g. /root/src
    singlePathFolders = strtok(folders, " ");  // Split in space
    while (singlePathFolders != NULL) {
        argumentsV[argumentsC++] = "-c";
        argumentsV[argumentsC++] = singlePathFolders;
        singlePathFolders = strtok(NULL, " ");
    }

    char *singlePath;                 // Contains the splited files, e.g. /root/src/main/main.c
    singlePath = strtok(files, " ");  // Split in space
    while (singlePath != NULL) {
        argumentsV[argumentsC++] = singlePath;
        singlePath = strtok(NULL, " ");
    }

    argumentsV[argumentsC++] = "-a";
    argumentsV[argumentsC++] = NULL;
    runProgram(argumentsV);
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
}

void getAnalytics() {
    char *path[] = {"/root/bin/report", NULL};
    executableChecks(path[0]);
    int pid = fork();
    if (pid == -1)  // Error in forking
        return;
    else if (pid == 0) {  // Child section
        execvp(path[0], path);
    } else {
        wait(pid);
    }
}

/**
 * Run program as a saparate process
 */
int runProgramAsProcess(char **path) {
    executableChecks(path[0]);
    system(strcat(path[0], " &"));
    return 0;
}

/**
 * Run program as child and send stderr and stdout to pipe
 */
int runProgram(char **path) {
    executableChecks(path[0]);
    int pipefd[2];
    pipe(pipefd);
    pid_t pid = fork();
    if (pid == -1)  // Error in forking
        return 1;
    else if (pid == 0) {  // Child section
        close(pipefd[0]);
        execvp(path[0], path);
        close(pipefd[1]);
    } else {
        wait(pid);
    }
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
    char *tokens[PATH_MAX];
    char *token = strtok(path, "/");
    int i = 0;
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, "/");
    }

    i = 0;
    char *outString = malloc(MAX_INPUT_LENGHT);

    // Adds slash in "/root/bin/"             ^
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
    strcat(command, processPath);
    strcat(command, " && make clean && make build");
    system(command);
}

/**
 * Get path and file, merge and check for integrity, if not rebuild.
 * e.g.
 * thisBaseName = "/root/bin/"
 */
int checkIntegrity(char *file) {
    int r = executableChecks(file);
    if (r == 2 || r == 3 || r == 4 || r == 5)
        return 1;  // Rebuild
    return 0;      // Do not rebuild
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
 */
int pathIsExecutable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR);
}

/**
 * Returns if path is a file
 */
int pathIsFile(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0);
}

/**
 * Returns if path is a folder
 */
int pathIsFolder(char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0)
        return 0;
    return S_ISDIR(sb.st_mode);
}

/**
 * Returns if path is a link
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
 */
void ignoreSignal(int signal) {
}

/**
 * Reras
 */
void runReport(int signal) {
    getAnalytics();
}

/**
 * Check if string is an integer or not
 */
int stringIsInt(char *string) {
    char *newString;
    long number = strtol(string, &newString, 10);  // Base 10

    if (*newString != '\0' || newString == string) {
        printf("Scrivi un numero oppure lascia vuoto per il valore di default\n");
    } else {
        return TRUE;
    }
    return FALSE;
}


int isProcessNumberOk(char *string) {
    char *newString;
    int number;
    if (string[0] == '\0')
        number = 3;
    else
        number = atoi(string);
    printf("-%s-, %d\n", string, number);
    if (number > 15) {
        printf("Il numero di processi P e' troppo alto, scegli un numero minore o uguale a 15.\n");
        return FALSE;
    } else if(number <= 0) {
        printf("Il numero di processi P e' troppo basso, scegli un numero piu' grande di 0.\n");
        return FALSE;
    }else{
        return TRUE;
    }
        
    return FALSE;
}


int isProcessNumberOkV2(char *string1, char *string2) {
    char *newString;
    int number1, number2;
    if (string1[0] == '\0')
        number1 = 3;
    else
        number1 = atoi(string1);

    if (string2[0] == '\0')
        number2 = 4;
    else
        number2 = atoi(string2);

    if (number1 + (number1 * number2) >= 165) {
        printf("Il numero di processi P*Q e' troppo alto, scegli un numero in modo che P*Q sia piu' piccolo di 150\n");
        return FALSE;
    } else if (number2 <= 0) {
        printf("Il numero di processi Q e' troppo basso, scegli un numero di processi piu' grande di 1\n");
        return FALSE;
    }
    return TRUE;
}

//clean buffers

void clean(int msgKey, string path) {
    key_t key = ftok(path, msgKey); /* key to identify the queue */
    if (key < 0) printf("key not gotten...\n");

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0) printf("no access to queue...\n");

    if (msgctl(qid, IPC_RMID, NULL) < 0) /* NULL = 'no flags' */
        printf("trouble removing queue...\n");
}