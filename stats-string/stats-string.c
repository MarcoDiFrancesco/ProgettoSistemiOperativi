#include "stats-string.h"

//QUEL 5 VA SOSTITUITO DA UNA LABEL DEL TIPO DEFINE 5 CATHEGORIES

char **statsToString(int *values){

    char **str = (char **)malloc(5 * sizeof(char *));
    for(int i = 0; i < 5; ++i){
        str[i] = (char *)malloc(12 * sizeof(int));
    }

    for(int i = 0; i < 5; ++i){
        sprintf(str[i], "%d", values[i]);
    }

    return str;
}


int *getValuesFromString(char **str){
    int *values = (int *)malloc(5 * sizeof(int));
    
    for(int i = 0; i < 5; ++i){
        values[i] = atoi(str[i]);
    }

    return values;
}