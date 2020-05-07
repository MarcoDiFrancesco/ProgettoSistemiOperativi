#include "counter.h"

int N=3;
int M=4;

string *readAndWait(int pipe[], pid_t son);
int writePipe(int pipe[],string *msg);

int main(int argc,string argv[]){
    //tester
    int tt=open("log.txt",O_WRONLY);
    int tt2[2];
    tt2[1]=tt;
    //tester


    int return_value;
    string toRead=argv[1];
    //temporaneo per testare 
    string files[argc-1];
    int i;
    for(i=1;i<argc;i++){
        files[i-1]=argv[i];
    }
    //tmp
    int fp=open(toRead,O_RDONLY);
    int dim = lseek(fp,0,SEEK_END);
    int part=dim/M;
    //pipes
    int p_c[N][2];
    int q_p[M][2];
    //counters
    int j;
    printf("Process C pid=%d\n",getpid());

    for(i=0;i<N;i++){
        //creo N processi di tipo P
        //creo pipe fra C e P
        pipe(p_c[i]);
        pid_t c_son=fork();
        if(c_son==-1){
            printf("error occurred at line 35\n");
            return_value=35;
        }else{
            if(c_son==0){
                //processo P
                printf("P created pid=%d ppid=%d\n",getpid(),getppid());

                //creo M processi di tipo Q
                for(j=0;j<M;j++){

                    //creo la pipe fra P e Q
                    pipe(q_p[j]);
                    int p_son=fork();
                    if(p_son==-1){
                        printf("error occurred at line 46\n");
                        return_value=46;
                    }else{
                        if(p_son==0){
                            //processo Q
                            printf("\tQ created pid=%d ppid=%d\n",getpid(),getppid());

                            //logica processo  q
                            int* counter=processoQ(part*j,part*(j+1),files[0]);
                            //printf("iniz=%d fin=%d",part*j,part*(j+1));
                            string *message=statsToString(counter);
                            //printf("%s \n",message[0]);
                            //int err=writePipe(q_p[j],message);
                            int err=writePipe(tt2,message);
                            printf("??? %d %s", errno, strerror(errno));
                            exit(0);
                        }else{
                            //successive parti del processo P
                            string *qTop=readAndWait(q_p[j],p_son);
                            for(i=0;i<CLUSTER;i++){
                                printf("%s ",qTop[i]);
                            }
                            printf("\n");
                            fflush(stdout);
                            //waitpid(p_son,NULL,0);
                        }
                        
                    }
                }
                //potenziale uscita del processo P
                exit(0);
            }else{
                //successive parti del processo C
                waitpid(c_son,NULL,0);
            }
        }
    }
    return return_value;
}