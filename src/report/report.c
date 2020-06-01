#ifndef __REPORT_H_
#define __REPORT_H_

#include "report.h"

void read_result(int cluster, string *results){
    int i = 0,
        counter = 0,
        *nval = getValuesFromString(results);
    char input;

    printf("Choose your result format: values[v] percentage[p] both[a]");
    scanf(" %c", &input);
    while (input != 'v' && input != 'p' && input != 'a') {
        printf("Insert only [v], [p] or [a]");
        scanf(" %c", &input);
    }
    switch (input) {
        case 'v' :
            print_values(cluster, nval);
            break;
        case 'p' : 
            print_values(cluster, nval);
            break;
        case 'a' : 
            print_values(cluster, nval);
            printf("\n-----------");
            print_percentual(cluster, nval);
            break;
    }
}

void print_values(int cluster, int* results){
    int i = 0;

    for (i = 0; i < cluster; i++) {
        printf("\n> Numero di %s: %d", print_type(i), results[i]);
    }
}

void print_percentual(int cluster, int* results){
    int i = 0,
        dim = 0;

    for(i=0; i<cluster; i++){
        dim += results[i];
    }
    for(i=0; i<cluster; i++){
        printf("\n> percentuale di %s: %0.2f percento", print_type(i), ((float)results[i]/((float)dim))*100.0 );
    }
    printf("\n");
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

char **statsToString(int *values){

    int i;
    char **str = (char **)malloc(5 * sizeof(char *));
    for(i = 0; i < 5; ++i){
        str[i] = (char *)malloc(12 * sizeof(int));
    }

    for(i = 0; i < 5; ++i){
        sprintf(str[i], "%d", values[i]);
    }

    return str;
}

int *getValuesFromString(char **str){
    int *values = (int *)malloc(CLUSTER * sizeof(int));
    int i;
    for(i = 0; i < CLUSTER; ++i){
        values[i] = atoi(str[i]);
    }

    return values;
}

//message functions

map readerMessage() {

    //string *ret=(char **)malloc(CLUSTER * sizeof(char *));
    int i=0, j, nFiles;
    //for (i = 0; i < CLUSTER; ++i) {
        //ret[i] = (char *)malloc(12 * sizeof(int));
    //}
    map ret = malloc(sizeof(FileMap));

    key_t key= ftok(PathName, ProjectId); /* key to identify the queue */
    if (key < 0) report_and_exit("key not gotten...");

    int qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
    if (qid < 0) report_and_exit("no access to queue...");

    printf("key : %d\nqid : %d\n", key, qid);

    queuedMessage msg; /* defined in queue.h */
    if (msgrcv(qid, &msg, sizeof(msg), 1, MSG_NOERROR) < 0)   puts("msgrcv (num) trouble...");
    printf("%s (num) received as type %i\n", msg.payload, (int) msg.type);
    string tmp = malloc(sizeof(100));
    strcpy(tmp, msg.payload);
    nFiles = atoi(tmp);

    /*int types[CLUSTER*nFiles + nFiles];
    for(i=0; i<CLUSTER*nFiles + nFiles; i++){
        types[i] = i+2;
    }*/

    int counter = 2;
    for(j=0; j<nFiles; j++){
        //salvataggio nome file
        if (msgrcv(qid, &msg, sizeof(msg), counter, MSG_NOERROR) < 0)   puts("msgrcv (name) trouble...");
        printf("%s (name) received as type %i\n", msg.payload, (int) msg.type);
        ret[j].name=malloc(sizeof(msg.payload));
        strcpy(ret[j].name, msg.payload);
        counter++;
        for (i = 0; i < CLUSTER; i++) {
            //salvataggio dati file
            if (msgrcv(qid, &msg, sizeof(msg), counter, MSG_NOERROR) < 0)   puts("msgrcv trouble...");
            printf("%s (%d) received as type %i\n", msg.payload, j, (int) msg.type);
            strcpy(tmp, msg.payload);
            ret[j].stats[i] = atoi(tmp);
            counter++;
        }
    }
    /** remove the queue **/
    if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");

    return ret;
    }

    void report_and_exit(const char* msg) {
    perror(msg);
    exit(-1); /* EXIT_FAILURE */
}

#endif
