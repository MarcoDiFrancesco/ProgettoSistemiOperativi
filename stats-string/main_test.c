#include <stdio.h>
#include <stdlib.h>


char *toString(int *values){

    char *buf = (char *)malloc((5 + (5-1)) * sizeof(char));
    int count = 0;

    for(int i = 0; i < 5; ++i){
        buf[count] = values[i] + '0';
        ++count;
        buf[count] = ' ';
        if(i < 4){
            ++count;
        }
    }

    return buf;
}

int *getValues(char *string){
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

int main(int argc, char *argv[]){


    int *stats = malloc(5*sizeof(int));

    int *n = malloc(5*sizeof(int));

    stats[0] = 4;
    stats[1] = 3;
    stats[2] = 3;
    stats[3] = 0;
    stats[4] = 1;

    printf("%s\n", toString(stats));

    n = getValues(toString(stats));

    for(int i = 0; i < 5; ++i){
        printf("%d\n", n[i]);
    }

    return 0;
}