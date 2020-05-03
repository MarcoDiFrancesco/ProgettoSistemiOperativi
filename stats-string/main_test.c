#include <stdio.h>
#include <stdlib.h>


char **toString(int *values){

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

int main(int argc, char *argv[]){


    int *stats = malloc(5*sizeof(int));

    int *n = malloc(5*sizeof(int));

    stats[0] = 12;
    stats[1] = 5000000;
    stats[2] = 3;
    stats[3] = 0;
    stats[4] = 1;

    char **string = toString(stats);

    printf("STRINGA\n");
    for(int i = 0; i < 5; ++i){
        printf("%s ", string[i]);
    }

    printf("\n------------------------------------------------\n");

    n = getValuesFromString(toString(stats));

    printf("NUMERI\n");

    for(int i = 0; i < 5; ++i){
        printf("%d\n", n[i]);
    }


    return 0;
}