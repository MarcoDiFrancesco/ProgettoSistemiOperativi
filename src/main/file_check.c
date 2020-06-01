#ifndef _MAIN_H_
#define _MAIN_H_

#include "main.h"

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