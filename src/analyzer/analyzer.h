#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>

#define READ 0
#define WRITE 1
#define CLUSTER 5

#define BOOL char
#define FALSE 0
#define TRUE 1
#define MAXLEN 12*sizeof(int)

typedef char *string;

//costanti funzionali
/*int N = 3;
int M = 4;
int n_arg = 1;*/

// TODO: merge with main_functions.h
#ifndef _LINKED_LIST_
#define _LINKED_LIST_
struct LinkedList {
    char *str;
    struct LinkedList *next;
};
typedef struct LinkedList *node;
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif


BOOL isTxt(string file, int length);
BOOL isCpp(string file, int length);
BOOL isC(string file, int length);
BOOL isJava(string file, int length);
BOOL isPy(string file, int length);

int file_exists(char *filename);
int is_folder(char *folder);

void getArgument(char *argName, int argc, char **argv, int *n_arg, char **processesStr, int *processInt);

#endif
