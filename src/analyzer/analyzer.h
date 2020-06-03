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

//costanti funzionali
/*int N = 3;
int M = 4;
int n_arg = 1;*/

typedef char * string;

// https://www.hackerearth.com/practice/data-structures/linked-list/singly-linked-list/tutorial/
struct LinkedList {
    char *str;
    struct LinkedList *next;
};

typedef struct LinkedList *node;

BOOL isTxt(string file, int length);
BOOL isCpp(string file, int length);
BOOL isC(string file, int length);
BOOL isJava(string file, int length);
BOOL isPy(string file, int length);

//funzione per cartelle

node createNode();
node addNode(node head, char *new_str);
int file_exists(char *filename);
int is_folder(char *folder);
char *concat(const char *s1, const char *s2, const char *s3);
node listFiles(char *path);


#endif
