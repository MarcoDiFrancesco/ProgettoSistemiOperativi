#ifndef _MAIN_H_
#define _MAIN_H_

#include <time.h>

#include "main.h"
/** ONLY FOR TESTING
 * Select delay in seconds 
 */
int delay(double delay) {
    time_t start = time(NULL);
    time_t current;
    do {
        time(&current);
    } while (difftime(current, start) < delay);
    return delay;
}

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
 * This function runs the program specified on path and wait for it to end.
 */
int runProgramAndWait(char **path) {
    // Check for executable errors
    executableChecks(path[0]);
    int pid = fork();

    if (pid == -1) {  // Error in forking
        return 1;
    } else if (pid == 0) {  // Child section
        execvp(path[0], path);
    } else {
        printf("I'm the parent waiting for child:%d, I'm:%d\n", pid, getpid());
        // Wait for a process (the first that comes)
        wait(NULL);  // TODO: test with pid
        printf("Waited, now ending\n");
    }
    return 0;
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

#endif