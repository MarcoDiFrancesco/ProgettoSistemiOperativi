#include "analyzer.h"

BOOL isTxt(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 5] == '.' && file[length - 4] == 't' && file[length - 3] == 'x' && file[length - 2] == 't') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 4] == '.' && file[length - 3] == 't' &&
        file[length - 2] == 'x' && file[length - 1] == 't') {
        ret = TRUE;
    }
    return ret;
}

BOOL isCpp(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 5] == '.' && file[length - 4] == 'c' && file[length - 3] == 'p' && file[length - 2] == 'p') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 4] == '.' && file[length - 3] == 'c' &&
        file[length - 2] == 'p' && file[length - 1] == 'p') {
        ret = TRUE;
    }
    return ret;
}

BOOL isC(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 3] == '.' && file[length - 2] == 'c') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 2] == '.' && file[length - 1] == 'c') {
        ret = TRUE;
    }
    return ret;
}

BOOL isJava(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' &&
        file[length - 6] == '.' && file[length - 5] == 'j' &&
        file[length - 4] == 'a' && file[length - 3] == 'v' &&
        file[length - 2] == 'a') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 5] == '.' && file[length - 4] == 'j' &&
        file[length - 3] == 'a' && file[length - 2] == 'v' &&
        file[length - 1] == 'a') {
        ret = TRUE;
    }
    return ret;
}

BOOL isPy(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 4] == '.' && file[length - 3] == 'p' && file[length - 2] == 'y') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 3] == '.' &&
        file[length - 2] == 'p' &&
        file[length - 1] == 'y') {
        ret = TRUE;
    }
    return ret;
}

/**
 * Get n or m argument from command.
 * e.g. /root/bin/analyzer -n 5 -m 7
 * Result:
 * n = 5
 * m = 7
 */
void getArgument(char *argName, int argc, char **argv, int *n_arg, char **processesStr, int *processInt) {
    if (argc > 2 && strcmp(argv[1], argName) == 0) {
        *n_arg += 2;
        *processesStr = argv[2];
        *processInt = atoi(*processesStr);  // TODO: check if string is int
    } else if (argc > 4 && strcmp(argv[3], argName) == 0) {
        *n_arg += 2;
        *processesStr = argv[4];
        *processInt = atoi(*processesStr);  // TODO: check if string is int
    }
}