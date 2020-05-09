#include "error.h"

int set_error(int *ret, int value){
    *ret = value;
    return value;
}

int set_error_msg(int *ret, int err, char* msg){
    printf("%s", msg);
    return set_error(ret, err);
}