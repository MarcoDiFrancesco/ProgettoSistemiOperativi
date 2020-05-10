#include "error.h"

int set_error(int *ret, int value){
    if(value < 0){
        *ret += value;
        return value;
    }
    return 0;
    
}

int set_err_msg(int *ret, int err, char* msg){
    int er = set_error(ret, err);
    if(er<0){
        printf("%s", msg);
    }
    return er;
}