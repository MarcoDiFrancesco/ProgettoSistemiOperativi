#include "analyzer.h"

//funzioni

int C(int argc, string argv[]){
    
    execl("/bin/bash", "bash", "-c", "./bin/report", "-r", "testo", NULL);
}
