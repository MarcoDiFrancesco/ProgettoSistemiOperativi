#include <stdlib.h>
#include <sys/file.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define BOOL char
#define FALSE 0
#define TRUE 1

#define BUFFSIZE 1000

int readFile(char* filename, char* filedata, int start, int stop);

void countLetters(int dim, char* s, int* counter);

int* processoQ(int from, int to, char* fname);