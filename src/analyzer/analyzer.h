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


#endif