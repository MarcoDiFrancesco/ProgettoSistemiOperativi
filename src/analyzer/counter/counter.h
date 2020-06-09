#ifndef _COUNTER_H_
#define _COUNTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>
#include <ctype.h>
#include <signal.h>

#define READ 0
#define WRITE 1
#define MAXLEN 12*sizeof(int)
#define BOOL char
#define FALSE 0
#define TRUE 1
#define BUFFSIZE 1000
#define WRITESIGNAL 1234

//define per il cluster
#define CLUSTER 8
#define UPPERCASE 0
#define LOWERCASE 1
#define NUMBERS 2
#define SPACES 3
#define PUNCTUATION 4
#define PARENTHESIS 5
#define MATH_OPERATORS 6
#define OTHER CLUSTER - 1

//define per il sender
#define ProjectId 123
#define PathName  "/root/src/analyzer/counter/counter.h"  /* any existing, accessible file would do */
#define PathName2  "/root/src/analyzer/analyzer.h"
#define MAX_MSG_SIZE 100

//globals
int N;
int M;
pid_t *PIds;
pid_t **QIds;
BOOL boolP;
BOOL *boolQ;
int *checkQ;
int checkP;

//structures
typedef char * string;

typedef struct {
  long type;                  //must be of type long 
  char payload[MAX_MSG_SIZE]; // bytes in the message 
} queuedMessage;

struct FileMap{
  string name;
  unsigned long fileHash;
  int stats[CLUSTER];
} FileMap;

typedef struct FileMap *map;

//Function declaration

int writePipe(int pipe[],string *msg);

string *readAndWait(int pipe[], pid_t son);

int * filesPart(string *files,int num, int M);

int readFile(char* filename, char* filedata, int start, int stop);

void countLetters(int dim, char* s, int* counter);

int **processoQ_n (int *range, int *dims, char** fname, 
                   int n, int q_loop, int index);

char **statsToString(int *values);

char ***statsToStringN(int **values, int size);

int *getValuesFromString(char **str);

int **getValuesFromStringN(char ***str, int size);

void printError(int code);

int *filesDim(string *files, int num);

int writePipeN(int pipe[], string **msg, int size);

string **readAndWaitN(int pipe[], int size);

void storeOnMap(map fileData, int **values, int size, int index);

unsigned long computeHash(string fname, int dim, BOOL compare);

int countDigits(unsigned long n);

int fileDim(string file);

void nl();

//process function

int processP(pid_t c_son, int pipe_c[][2], int pipe_q[][2], string *file_P,
             int ceil, int fileIndex, int *part, int *fdim, 
             int index_p,int file_per_p, int f_Psize);

int processQ(int *range, int *dims, char** fname, int f_Psize, 
             int q_loop, int index, int pipe_q[]);

//IPC functions

void report_and_exit(const char* msg);

void sender(map data, int mapDim);


//signal functions

void signalhandler(int sig);

void sighandlerP(int sig);

void sigHandlerQ(int sig);

void ignore(int sig);

//funzioni messaggi per aggiungere roba

void report_and_exit(const char* msg);

void sendConfirm(string messaggio, int projID, string path);

string recConfirm(int projID, string path);

#endif