#include "read_file.h"

int main(){
    node l = listFiles("../analyzer/counter");

    while( l->next != NULL){
        printf("\n> %s", l->str);
        l = l->next;
    }

    return 0;
}