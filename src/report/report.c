#include "report.h"

void read_result(map results, int numFile) {
    char input;

    printf("Vuoi le statistiche di tutti i file [a]\ndi un file singolo [u]\noppure le statistiche complessive [s]?\n");
    scanf(" %c", &input);
    while (input != 'a' && input != 'u'&& input != 's') {
        printf("Inserisci solamente [a], [u] o [s]");
        scanf(" %c", &input);
    }
    long *temp;
    switch (input) {
        case 'a' :
            printAll(results, numFile);
            break;
        case 'u' : 
            printSingle(results, selectFile(results, numFile));
            break;
        case 's':
            temp = computeOverall(results, numFile);
            printOverall(temp);
            break;
    }
    free(temp);
}

//new print functions

void printAll(map results,int numFile){
    int i;
    char input;
    printf("Scegli il formato di visualizzazione: per valore[v] percentuale[p] o entrambi[e]");
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
                printf("\nNome del file : %s\n",results[i].name);
                print_values(results[i].stats);
                printf("\n-----------\n");
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
    string input = malloc(7);
    int intput;
    fgets(input, 7, stdin);
    intput = atoi(input);
    while (!(intput >= 1 && intput <= numFile)) {
        printf("Inserisci un indice di un file sopra citato\n");
        fflush(stdout);
        fflush(stdin);
        fgets(input, 7, stdin);
        intput = atoi(input);
    }
    return intput-1;
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
            printf("\n-----------\n");
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
        case UPPERCASE : return "lettere maiuscole";
        case LOWERCASE : return "lettere minuscole";
        case NUMBERS : return "numeri";
        case SPACES : return "spazi";
        case PUNCTUATION : return "punteggiatura";
        case PARENTHESIS : return "parentesi";
        case MATH_OPERATORS : return "operatori matematici";
        case OTHER : return "altri simboli";
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

long *computeOverall(map results, int numFiles) {
    int i, j;
    long *overall = (long *)malloc(CLUSTER * sizeof(long));
    for (j = 0; j < CLUSTER; ++j) {
        overall[j] = 0;
    }
    for (i = 0; i < numFiles; ++i) {
        for (j = 0; j < CLUSTER; ++j) {
            overall[j] += results[i].stats[j];
        }
    }
    return overall;
}

void printOverall(long *values) {
    printf("Hai scelto di stampare le statistiche generali\n");
    char input;
    printf("Scegli il formato di visualizzazione: per valore[v] percentuale[p] o entrambi[e]");
    scanf(" %c", &input);
    while (input != 'v' && input != 'p' && input != 'e') {
        printf("Insermento non valido, inserire esclusivamente [v], [p] o [e]");
        scanf(" %c", &input);
    }
    switch (input) {
        case 'v' :
            print_values_long(values);
            break;
        case 'p' : 
            print_percentual_long(values);
            break;
        case 'e' : 
            print_values_long(values);
            printf("\n-----------");
            print_percentual_long(values);
            break;
    }
}

//message functions

map readerMessage(int *numFileRet) {

    kill(0, SIGUSR2);
    printf("Per iniziare a ricevere dati è necessiario lanciare analyzer [bin/analyzer]\n\n");

    int i=0, j, nFiles;
    key_t key= ftok(PathName, ProjectId);
    if (key < 0) report_and_exit("key not gotten...");

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0) report_and_exit("no access to queue...");

    queuedMessage msg; 
    if (msgrcv(qid, &msg, sizeof(msg), 1, MSG_NOERROR) < 0)   puts("msgrcv (num) trouble...");

    string tmp = malloc(sizeof(100));
    strcpy(tmp, msg.payload);
    nFiles = atoi(tmp);
    *numFileRet = nFiles;
    map ret = malloc(nFiles*sizeof(FileMap));
    int counter = 2;
    for(j=0; j<nFiles; j++){
        //salvataggio nome file
        if (msgrcv(qid, &msg, MAX_MSG_SIZE, counter, MSG_NOERROR) < 0)   puts("msgrcv (name) trouble...");
        ret[j].name=malloc(strlen(msg.payload));
        strcpy(ret[j].name, msg.payload);
        counter++;
        for (i = 0; i < CLUSTER; i++) {
            //salvataggio dati file
            if (msgrcv(qid, &msg, sizeof(msg), counter, MSG_NOERROR) < 0)   puts("msgrcv trouble...");
            strcpy(tmp, msg.payload);
            ret[j].stats[i] = atoi(tmp);
            counter++;
        }
    }
    if (msgctl(qid, IPC_RMID, NULL) < 0) 
        report_and_exit("trouble removing queue...");

    return ret;
}

void report_and_exit(const char* msg) {
    perror(msg);
    exit(-1);
}

void sighandler(int sig){

}

void print_values_long(long* results){
    int i = 0;

    for (i = 0; i < CLUSTER; i++) {
        printf("\n> Numero di %s: %ld", print_type(i), results[i]);
    }
    printf("\n");
}

void print_percentual_long(long* results){
    int i = 0, dim = 0;
    for(i=0; i<CLUSTER; i++){
        dim += results[i];
    }
    for(i=0; i<CLUSTER; i++){
        printf("\n> percentuale di %s: %0.2f percento", print_type(i), ((float)results[i]/((float)dim))*100.0 );
    }
    printf("\n");
}

void sendConfirm(string messaggio, int projID, string path) {
    key_t key = ftok(path, projID);
    if (key < 0) {
        printf("err: %s\n", strerror(errno));
        report_and_exit("couldn't get key...");
    }

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0)
        report_and_exit("couldn't get queue id...");

    queuedMessage msg;
    strcpy(msg.payload, messaggio);
    msg.type = 1;

    msgsnd(qid, &msg, strlen(msg.payload) + 1, MSG_NOERROR | IPC_NOWAIT);
}

string recConfirm(int projID, string path) {
    key_t key = ftok(path, projID);
    if (key < 0) {
        printf("err: %s\n", strerror(errno));
        report_and_exit("couldn't get key...");
    }

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0)
        report_and_exit("couldn't get queue id...");

    queuedMessage msg;
    if (msgrcv(qid, &msg, MAX_MSG_SIZE, 1, MSG_NOERROR) < 0) puts("AAAAAAAAAAAAAAAAAAAAAAAA trouble...");

    if (msgctl(qid, IPC_RMID, NULL) < 0)
        report_and_exit("trouble removing queue...");

    string ret = malloc(strlen(msg.payload));
    strcpy(ret, msg.payload);

    return ret;
}

/**
 * Get where current program is executed and make file
 */
void makeFiles(char *processPath) {
    char command[4096 + 4 + 29];
    strcat(command, "cd ");
    strcat(command, processPath);
    strcat(command, " && make clean && make build");
    system(command);
}