#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#define FALSE 0
#define TRUE 1
typedef char *string;

/**
 * Show warnings like "Detected another directory" or "Signal ignored".
 */
#define SHOW_WARNING 1

/**
 * Show information like "I'm forking", "I waited for {pid}".
 */
#define SHOW_INFO 1

/**
 * Used debugging features like: allow signals to kill the process.
 */
#define DEBUG 1

/**
 * Max size when reading from input. Limitaion given by C99.
 * 
 * Credits: https://stackoverflow.com/a/11488687/7924557
 */
#define MAX_INPUT_LENGHT 4096

/**
 * Max size of a path.
 * 
 * Credits: https://stackoverflow.com/a/9449307/7924557
 */
#define PATH_MAX 4096

struct LinkedList {
    char *str;
    struct LinkedList *next;
};
typedef struct LinkedList *node;

node createNode();
node addNode(node head, char *new_str);
char *concat(const char *s1, const char *s2, const char *s3);
node listFiles(char *path);
void removeNewline(char *string);
void splitAndSendPaths(char *string);
char *getSelfProcessPath();
void getAnalytics();
int runProgram(char **path);
int runProgramAndWait(char **path);
char *baseName(char *path);
void makeFiles(char *processPath);
int checkIntegrity(char *file, char *processPath);
int executableChecks(char *path);
void printError(int errNumber);
int pathIsExecutable(char *path);
int pathIsFile(char *path);
int pathIsFolder(char *path);
int pathIsLink(char *path);
char *concatPaths(char *dir, char *file);
void ignoreSignal(int signal);