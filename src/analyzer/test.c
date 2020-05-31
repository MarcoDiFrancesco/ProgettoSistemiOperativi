#include "test.h"

//funzioni

int C(int argc, string argv[]){
    string arguments[argc];
    int i;
    for(i=0;i<argc;i++){
        arguments[i]=arguments[i];
    }
    execv("../counter/main", arguments);
}
