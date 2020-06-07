#ifndef __REPORT_H_
#define __REPORT_H_

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

#define BOOL char
#define FALSE 0
#define TRUE 1
#define MAX_BUF_SIZE 1000

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
#define PathName  "../src/analyzer/counter/counter.h" /* any existing, accessible file would do */
#define MAX_MSG_SIZE 100
#define WRITESIGNAL 1234


typedef char * string;

typedef struct {
  long type;                  //must be of type long 
  char payload[MAX_MSG_SIZE]; // bytes in the message 
} queuedMessage;

struct FileMap{
  string name;
  int fileHash;
  int stats[CLUSTER];
} FileMap;

typedef struct FileMap *map;

enum simboli{lettere, numeri, spazi, punteggiatura, altro};

void read_result(map results,int numFIle);

void print_values(int* results);

void print_percentual(int* results);

string print_type(int n);

//phil functions

string *statsToString(int *values);

int *getValuesFromString(char **str);

int *computeOverall(map results, int numFiles);

void printOverall(int *values);

//message functions
void report_and_exit(const char* msg);

map readerMessage(int *numFileRet);

//funzioni stampa

void printAll(map results,int numFile);

void printSingle(map results, int fileNum);

int selectFile(map results, int numFile);

void printNames(map results, int nFile);

void sighandler(int sig);

#endif