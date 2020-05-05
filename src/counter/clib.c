#ifndef _COUNTER_H_
#define _COUNTER_H_

#include "counter.h"


string readAndWait(int pipe[], pid_t son){
    close(pipe[WRITE]);
    string msg;
    msg=malloc(MAXLEN);
    int rd=read(pipe[READ],msg, MAXLEN);
    close(pipe[READ]);
    int err=waitpid(son,NULL,0);
    return msg;
}

int writePipe(int pipe[],string msg){
    int ret=0;//per eventuali errori
    close(pipe[READ]);
    write(pipe[WRITE],msg,strlen(msg)+1);
    close(pipe[WRITE]);
    return ret;
}

#endif
