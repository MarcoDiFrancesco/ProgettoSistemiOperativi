#include "stats-string.h"

//QUEL 5 VA SOSTITUITO DA UNA LABEL DEL TIPO DEFINE 5 CATHEGORIES

char *statsToString(int *values){

    char *buf = (char *)malloc((5 + (5-1)) * sizeof(char));
    int count = 0;

    for(int i = 0; i < 5; ++i){
        buf[count] = values[i] + '0';
        ++count;
        buf[count] = ' ';
        if(i < (5 - 1)){
            ++count;
        }
    }

    return buf;
}


int *getValuesFromString(char *string){
    int *values = (int *)malloc(5 * sizeof(int));
    int count = 0;

    int i = 0;
    while(string[i] != '\0'){
        if(string[i] != ' '){
            values[count] = string[i] - '0';
            ++count;
        }
        ++i;
    }

    return values;
}