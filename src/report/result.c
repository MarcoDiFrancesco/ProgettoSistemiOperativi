#include "result.h"

void read_result(int cluster, string results){
    int i = 0,
        counter = 0,
        *nval = getValuesFromString(&results);
    char input;

    printf("wich result format: values[v] percentual[p] both[a]");
    scanf("%c", input);
    while(input != 'v' || input != 'p' || input != 'a'){
        printf("insert only [v] [p] or [a]");
        scanf("%c", input);
    }

    switch(input){
        case 'v' :
            print_values(cluster, nval);
            break;
        case 'p' : 
            print_values(cluster, nval);
            break;
        case 'a' : 
            print_values(cluster, nval);
            printf("\n-----------");
            print_values(cluster, nval);
            break;
    }
}

void print_values(int cluster, int* results){
    int i = 0;

    for(i=0; i<cluster; i++){
        printf("\n> numero di %s: %d", print_type(i), results[i]);
    }
}

void print_percentual(int cluster, int* results){
    int i = 0,
        dim = 0;

    for(i=0; i<cluster; i++){
        dim += results[i];
    }

    for(i=0; i<cluster; i++){
        printf("\n> percentuale di %s: %d%", print_type(i), (results[i]/dim)*100 );
    }
}

string print_type(int n){
    switch(n){
        case 0 : return "lettere";
        case 1 : return "numeri";
        case 2 : return "spazi";
        case 3 : return "punteggiatura";
        case 4 : return "altri simboli";
    }
};

//phin functions

int *getValuesFromString(char **str){
    int *values = (int *)malloc(CLUSTER * sizeof(int));
    int i;
    for( i = 0; i < CLUSTER; ++i){
        values[i] = atoi(str[i]);
    }

    return values;
}