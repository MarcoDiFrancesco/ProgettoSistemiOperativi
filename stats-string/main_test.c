#include <stdio.h>
#include <stdlib.h>


char **toString(int *values){

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

int main(int argc, char *argv[]){


    int *stats = malloc(5*sizeof(int));

    int *n = malloc(5*sizeof(int));

    stats[0] = 12;
    stats[1] = 5000000;
    stats[2] = 3;
    stats[3] = 0;
    stats[4] = 1;

    char **str = toString(stats);

    printf("STRINGA\n");
    for(int i = 0; i < 5; ++i){
        printf("%s ", str[i]);
    }

    printf("\n------------------------------------------------\n");

    n = getValuesFromString(toString(stats));

    printf("NUMERI\n");

    for(int i = 0; i < 5; ++i){
        printf("%d\n", n[i]);
    }


    return 0;
}