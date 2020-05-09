#include "counter.h"

int N=3;
int M=4;

int main(int argc,string argv[]){

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
                string *qTop[M];
                int dataCollected[CLUSTER];
                int g;
                for(g=0;g<CLUSTER;g++){
                    dataCollected[g]=0;
                }
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
                            string *message=statsToString(counter);
                            int err=writePipe(q_p[j],message);
                            exit(0);
                        }else{
                            //successive parti del processo P
                            qTop[j]=readAndWait(q_p[j],p_son);
                            int *tmp=getValuesFromString(qTop[j]);
                            for(g=0;g<CLUSTER;g++){
                                dataCollected[g]+=tmp[g];
                            }
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