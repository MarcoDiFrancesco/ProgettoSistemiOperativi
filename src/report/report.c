#include "report.h"

void read_result(map results,int numFile){
    /*int i = 0,
        counter = 0,
        *nval = getValuesFromString(results);*/
    char input;

    printf("Vuoi le statistiche di tutti i file [a]\no di un file singolo [u]?\n");
    scanf(" %c", &input);
    while (input != 'a' && input != 'u') {
        printf("Inserisci solamente [a] o [u]");
        scanf(" %c", &input);
    }
    switch (input) {
        case 'a' :
            printAll(results, numFile);
            break;
        case 'u' : 
            printSingle(results, selectFile(results, numFile));
            break;
    }
    
    /*printf("Choose your result format: values[v] percentage[p] both[a]");
    scanf(" %c", &input);
    while (input != 'v' && input != 'p' && input != 'a') {
        printf("Insert only [v], [p] or [a]");
        scanf(" %c", &input);
    }
    switch (input) {
        case 'v' :
            print_values(nval);
            break;
        case 'p' : 
            print_values(nval);
            break;
        case 'a' : 
            print_values(nval);
            printf("\n-----------");
            print_percentual(nval);
            break;
    }*/
}

//new print functions

void printAll(map results,int numFile){
    int i;
    char input;
    printf("Scegli il vormato di visualizzazione: per valore[v] percentuale[p] o entrambi[e]");
    scanf(" %c", &input);
    while (input != 'v' && input != 'p' && input != 'e') {
        printf("Insermento non valido, inserire esclusivamente [v], [p] o [e]");
        scanf(" %c", &input);
    }
    switch (input) {
        case 'v' :
            for(i=0;i<numFile;i++){
                printf("Nome del file : %s\n",results[i].name);
                print_values(results[i].stats);
            }
            
            break;
        case 'p' : 
            for(i=0;i<numFile;i++){
                printf("Nome del file : %s\n",results[i].name);
                print_percentual(results[i].stats);
            }
            break;
        case 'e' :
            for(i=0;i<numFile;i++){
                printf("Nome del file : %s\n",results[i].name);
                print_values(results[i].stats);
                printf("\n-----------");
                print_percentual(results[i].stats);
            }
            
            break;
    }
    
}

void printNames(map results, int nFile){
    int i;
    for(i=0; i<nFile; i++){
        printf("(%d) %s\n", i+1, results[i].name);
    }
}

int selectFile(map results, int numFile){
    printf("Questi sono i file disponibili.\n");
    printNames(results,numFile);
    printf("Inserisci l'indice del file di cui vuoi conoscere le statistiche\n> ");
    int input;
    scanf(" %d", &input);
    while (!(input >= 1 && input <= numFile)) {
        printf("Inserisci un indice di un file sopra citato\n");
        scanf(" %d", &input);
    }
    return input-1;
}

void printSingle(map results, int fileNum){
    printf("Il nome del file scelto è:\n> %s\n", results[fileNum].name);
    char input;
    printf("Scegli il vormato di visualizzazione: per valore[v] percentuale[p] o entrambi[e]");
    scanf(" %c", &input);
    while (input != 'v' && input != 'p' && input != 'e') {
        printf("Insermento non valido, inserire esclusivamente [v], [p] o [e]");
        scanf(" %c", &input);
    }
    switch (input) {
        case 'v' :
            print_values(results[fileNum].stats);
            break;
        case 'p' : 
            print_percentual(results[fileNum].stats);
            break;
        case 'e' : 
            print_values(results[fileNum].stats);
            printf("\n-----------");
            print_percentual(results[fileNum].stats);
            break;
    }
}

//old print functions

void print_values(int* results){
    int i = 0;

    for (i = 0; i < CLUSTER; i++) {
        printf("\n> Numero di %s: %d", print_type(i), results[i]);
    }
    printf("\n");
}

void print_percentual(int* results){
    int i = 0,
        dim = 0;

    for(i=0; i<CLUSTER; i++){
        dim += results[i];
    }
    for(i=0; i<CLUSTER; i++){
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

map readerMessage(int *numFileRet) {

    kill(0, SIGUSR1);

    int i=0, j, nFiles;
    key_t key= ftok(PathName, ProjectId); /* key to identify the queue */
    if (key < 0) report_and_exit("key not gotten...");

    int qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
    if (qid < 0) report_and_exit("no access to queue...");

    printf("key : %d\nqid : %d\n", key, qid);

    queuedMessage msg; 
    if (msgrcv(qid, &msg, sizeof(msg), 1, MSG_NOERROR) < 0)   puts("msgrcv (num) trouble...");
    printf("%s (num) received as type %i\n", msg.payload, (int) msg.type);
    string tmp = malloc(sizeof(100));
    strcpy(tmp, msg.payload);
    nFiles = atoi(tmp);
    *numFileRet = nFiles;
    map ret = malloc(nFiles*sizeof(FileMap));
    int counter = 2;
    for(j=0; j<nFiles; j++){
        //salvataggio nome file
        if (msgrcv(qid, &msg, MAX_MSG_SIZE, counter, MSG_NOERROR) < 0)   puts("msgrcv (name) trouble...");
        //printf("%s (name) received as type %i\n", msg.payload, (int) msg.type);
        ret[j].name=malloc(strlen(msg.payload));
        strcpy(ret[j].name, msg.payload);
        counter++;
        for (i = 0; i < CLUSTER; i++) {
            //salvataggio dati file
            if (msgrcv(qid, &msg, sizeof(msg), counter, MSG_NOERROR) < 0)   puts("msgrcv trouble...");
            printf("\terr (%d): %s\n", i, strerror(errno));
            //printf("%s (%d) received as type %i\n", msg.payload, j, (int) msg.type);
            strcpy(tmp, msg.payload);
            ret[j].stats[i] = atoi(tmp);
            counter++;
        }
        printf("err (f %d): %s\n", j, strerror(errno));
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

void sighandler(int sig){

}