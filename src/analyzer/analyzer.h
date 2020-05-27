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

typedef char * string;


int helloworld(void);