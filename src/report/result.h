#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BOOL char
#define FALSE 0
#define TRUE 1
#define CLUSTER 5


enum simboli{lettere, numeri, spazi, punteggiatura, altro};


typedef char * string;

void read_result(int cluster, string *results);

void print_values(int cluster, int* results);

void print_percentual(int cluster, int* results);

string print_type(int n);

//phil functions

string *statsToString(int *values);

int *getValuesFromString(char **str);
