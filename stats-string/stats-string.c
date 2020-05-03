#include "stats-string.h"

//QUEL 5 VA SOSTITUITO DA UNA LABEL DEL TIPO DEFINE 5 CATHEGORIES

char **statsToString(int *values){

    char **string = (char **)malloc(5 * sizeof(char *));
    for(int i = 0; i < 5; ++i){
        string[i] = (char *)malloc(12 * sizeof(int));
    }

    for(int i = 0; i < 5; ++i){
        sprintf(string[i], "%d", values[i]);
    }

    return string;
}


int *getValuesFromString(char **string){
    int *values = (int *)malloc(5 * sizeof(int));
    
    for(int i = 0; i < 5; ++i){
        values[i] = atoi(string[i]);
    }

    return values;
}