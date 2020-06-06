#ifndef _COUNTER_H_
#define _COUNTER_H_

#include "counternew.h"


string *readAndWait(int pipe[], pid_t son) {
    close(pipe[WRITE]);
    string *msg;
    msg = malloc(CLUSTER * sizeof(char *));
    int i;
    int rd;
    int err = 0;
    for (i = 0; i < CLUSTER; i++) {
        msg[i] = malloc(MAXLEN);
        rd = read(pipe[READ], msg[i], MAXLEN);
        if (rd == -1) {
            err = rd;
        }
        msg[i][rd] = 0;
    }
    //printf("(read)ERR=%d",err);
    close(pipe[READ]);
    waitpid(son, NULL, 0);
    return msg;
}

int writePipe(int pipe[],string *msg) {
    int ret = 0;//per eventuali errori
    //close(pipe[READ]);
    int i, err;
    
    for (i = 0; i < CLUSTER; i++) {
        err = write(pipe[WRITE], msg[i], MAXLEN);
        if (err != 0) {
            ret = err;
        }
    }
    close(pipe[WRITE]);
    return ret;
}

/**
 * Funzione che calcola le M-partizioni dei num file in input
 * e le inserisce in un vettore di interi, in modo che possano
 * essere recuperate nel momento in cui si vuole calcolare gli indici.
 * @param files lista di files da cui calcolare le M-partizioni.
 * @param num cardinalità della lista di partizioni.
 * @param M numero di partizioni.
 * @return vettore contenente le suddivisioni.
 */
int *filesPart(string *files, int num, int M) {
    int i;
    string toRead;
    int fp, dim;
    int *ret = malloc(num * sizeof(int));
    for (i = 0; i < num; i++) {
        toRead = files[i];
        fp = open(toRead, O_RDONLY);
        dim = lseek(fp, 0, SEEK_END);
        ret[i] = ceiling(dim, M);
        close(fp);
    }
    return ret;
}

//---------------------------ric functions------------------------------------
 
/**
 * Funzione che legge i caratteri in un dato intervallo passato e li ripone
 * in un buffer di lettura.
 * @param filename il file da cui estrarre la porzione.
 * @param filedata il buffer in cui riporre il contenuto letto.
 * @param start indice d'inizio.
 * @param stop indice di fine.
 * @return 0 in caso di successo, -1 in caso si siano verificati errori di lettura.
 */
int readFile(char* filename, char* filedata, int start, int stop) {
    int sk, rd;
    int ret;
    int file = open(filename, O_RDONLY);
    if (file >= 0) {
        sk = lseek(file, start * sizeof(char), SEEK_SET);
        if(sk != -1) {
            rd = read(file, filedata, stop-start);
        }
    }
    if (file >= 0 && sk >= 0 && rd >= 0)
        ret = 0;
    else
        ret = -1;
    return ret;
}

/**
 * Funzione che effettua il conteggio dei caratteri da una certa stringa
 * di dimensione data e ripone i risultati dell'analisi all'interno
 * di un vettore passato, utilizzando una catena di if else.
 * TODO: forse possiamo implementarla con degli switch?
 * @param dim dimensione della stringa da analizzare.
 * @param s buffer che contiene i caratteri da analizzare.
 * @param counter vettore in cui riporre i risultati.
 */
void countLetters(int dim, char* s, int* counter) {
    //printf("testo cont: ");
    int i = dim - 1;
    for (i; i >= 0; i--) {
        
        //printf("\t%c\n", s[i]);
        if (s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') {
            //lettera maiuscola o minuscola
            counter[0]++;
        } else if (s[i] >= '0' && s[i] <= '9') {
            //numero
            counter[1]++;
        } else if (s[i] == ' ') {
            //spazi
            counter[2]++;
        } else if (s[i] == '.' || s[i] == ',' || s[i] == ':' || 
                   s[i] == ';' || s[i] == '?' || s[i] == '!') {
            //punteggiatura
            counter[3]++;
        } else {
            counter[4]++;
        }
    }
}

int* processoQ(int from, int to, char* fname) {
    char* testo;
    int* stats;
    int i;
    int inizio = from;
    int fine = to;

    testo = malloc(fine * sizeof(char));
    stats = malloc(5 * sizeof(int));
    i = 0;

    for (i; i < CLUSTER; i++) {
        stats[i] = 0;
    }

    i = readFile(fname, testo, inizio, fine);

    countLetters(fine - inizio, testo, stats);


    if (i==0)
        return stats;
    else 
        return (int *)-1;
}
/**
 * Questa è la funzione in cui avviene l'analisi vera e propria della porzione di file. 
 * Inizialmente recupera gli indizi di inizio e fine degli n files da analizzare;
 * successivamente, prepara un vettore dinamico per il recupero dei dati analizzati.
 * A quel punto inizia il ciclo per effettuare il conteggio, dentro al quale viene allocata
 * dinamicamente (e subito rilasciata) la memoria per contenere la porzione di testo da
 * analizzare in quel momento. A quel punto si legge il file  e si ripone il contenuto da
 * analizzare nel buffer apposito, e si conserva il valore di ritorno per questioni
 * di debug.
 * 
 * @param range Vettore che contiene, per ogni file dato in input alla chiamata del main,
 *              il valore ceil(dimensioneFile/M).
 * @param dims Vettore che contiene la dimensione di ogni file.
 * @param fname Vettore di stringhe che contiene i file che il processo P chiamante di questo
 *              Q dovrà analizzare.
 * @param n Numero di elementi di fname.
 * @param q_loop Indice dell'iterazione corrispondente a questo processo Q.
 * @param index Prodotto tra l'iterazione tra i processi P e il numero di file che ogni P può
 *              analizzare; utile per sapere a quale elemento siamo arrivati nelle liste che non
 *              distinguono i file tra i vari processi P.
 * @param M Numero complessivo di sottoprocessi Q.
 *              
 * @return Vettore di interi che contiene i risultati dell'analisi, -1 in caso d'errore.
 */
int **processoQ_n (int *range, int *dims, char** fname, int n, int q_loop, int index, int M) {
    char *testo;
    int **stats;
    int i, j, k, alloc_value;
    int inizio[n], fine[n];
    i = 0;
/*
    for (i = 0; i < n; i++) {
        printf("\tsto analizzando = %s\n", fname[i]);
    }*/

    //printf("\tl'indice per recuperare gli indici è %d\n", index);

    i = 0;

    // Recupero degli indici di inizio e fine, per ciascuno degli n file: 
    // si parte da index e si itera n volte.
    for (j = index; j < (index + n); j++) {

        // Controllo per gestire i casi in cui la dimensione del file non
        // è un multiplo di M.
        inizio[i] = range[j]*q_loop;
        if ((range[j] * (q_loop + 1) <= dims[j])) {
            // printf("%d",range[j]*q_loop + j);
            fine[i] = range[j]*(q_loop + 1);
        } else {
            fine[i] = dims[j]; 
        } 
        
        // Controllo per i file di dimensione inferiore a M - 1.
        if (inizio[i] > fine[i]) {
            inizio[i] = fine[i] = 0;
        }   
    
        /*if(inizio[i] != fine[i]) {
            printf("\tinizio=%d, fine=%d\n",inizio[i],fine[i]);
        } else {
            printf("\t---------\n");
        }*/
        ++i;
    }

    //stats = malloc(sizeof(int));
    stats = (int **)malloc(n * sizeof(int *));

    for (i = 0; i < n; i++) {
        ///printf("\t%d\n", i);
        stats[i] = malloc(CLUSTER * sizeof(int));
        for (j = 0; j < CLUSTER; ++j) {
           // printf("\t\t%d\n", j);
            stats[i][j] = 0;
        }
    }
    k = index;
    // In questa iterazione si assegna dinamicamente la dimensione del buffer di testo
    // e la si libera subito, per evitare sprechi di memoria. La funzione readFile()
    // deposita una porzione di file, ricavata sulla base degli indici, nel buffer 
    // appena allocato, conservando il ritorno in caso di errori.
    for (j = 0; j < n; j++) { 
        alloc_value = ceiling(dims[k + j], M); 
        testo = malloc(alloc_value);
        i = readFile(fname[j], testo, inizio[j], fine[j]);
        //printf("\t%s\n",testo);
        countLetters(fine[j] - inizio[j], testo, stats[j]);
        free(testo);
        if (i < 0) {
            break;
        }
    }


    if (i == 0)
        return stats;
    else
        return (int **) - 1;
    
}


//---------------------------phil functions------------------------------------

/**
 * Funzione per trasformare un vettore di interi in una stringa che contenga
 * gli stessi valori.
 * @param values Vettori di interi da convertire.
 * @return La stringa che contiene gli stessi valori.
 */
char **statsToString (int *values) {

    char **str = (char **)malloc(CLUSTER * sizeof(char *));
    int i;
    for (i = 0; i < CLUSTER; ++i) {
        str[i] = (char *)malloc(12 * sizeof(int));
    }
    
    for (i = 0; i < CLUSTER; ++i) {
        sprintf(str[i], "%d", values[i]);
    }

    return str;
}

char ***statsToStringN(int **values, int size)
{

    char ***str = (char ***)malloc(size * sizeof(char **));
    int i, j;
    for (i = 0; i < size; ++i) {
        str[i] = (char **)malloc(CLUSTER * sizeof(char *));
        for (j = 0; j < CLUSTER; ++j) {
            str[i][j] = (char *)malloc(12 * sizeof(int));
        }
    }

    for (i = 0; i < size; ++i) {
        for (j = 0; j < CLUSTER; ++j) {
            sprintf(str[i][j], "%d", values[i][j]);
        }
    }

    return str;
}

/**
 * Funzione che recupera da una stringa dei valori numerici e li mette in un intero.
 * @param str Il vettore di stringhe che contiene il valore.
 * @return Il vettore di interi.
 */
int *getValuesFromString(char **str) {
    int *values = (int *)malloc(CLUSTER * sizeof(int));
    int i;
    for (i = 0; i < CLUSTER; ++i) {
        values[i] = atoi(str[i]);
    }

    return values;
}

int **getValuesFromStringN(char ***str, int size)
{
    int i, j;
    int **values = (int **)malloc(CLUSTER * sizeof(int *));
    for (i = 0; i < size; ++i) {
        values[i] = (int *)malloc(CLUSTER * sizeof(int));
        for (j = 0; j < CLUSTER; ++j) {
            values[i][j] = atoi(str[i][j]);
        }
    }

    return values;
}

/**
 * Funzione che stampa in modo graficamente leggibile un codice di errore
 * e relativa descrizione.
 * @param code Codice errore.
 */
void printError(int code) {
    printf("\n\n----------------------\n\n");
    printf("ERRNO = %d, error description = %s", code, strerror(code));
    printf("\n\n----------------------\n\n");
}

/**
 * Funzione per ottenere un vettore contenente la dimensione di tutti i num
 * file passati.
 * @param files riferimento alla lista di files di cui si vuole recuperare
 *              la dimensione.
 * @param num cardinalità della lista dei file passata.
 * @return puntatore alla memoria allocata al vettore di interi cin le dimensioni.
 */
int *filesDim(string *files, int num) {
    int i;
    string toRead;
    int fp;
    int *ret = malloc (num * sizeof(int));
    for (i = 0; i < num; i++) {
        toRead = files[i];
        fp = open(toRead, O_RDONLY);
        ret[i] = lseek(fp, 0, SEEK_END);
        close(fp);
    }
    return ret;
}

/**
 * Semplice funzione che restituisce il ceiling di due
 * valori interi.
 * @param first il dividendo
 * @param second il divisore
 * @return l'intero superiore della divisione
 */
int ceiling(int first, int second){
    int result = 0;
    if (first % second == 0){
        result = first / second;
    } else {
        result = (first / second) + 1;
    }
    return result;
}

int writePipeN(int pipe[], string **msg, int size)
{
    int ret = 0; //per eventuali errori
    //close(pipe[READ]);
    int i, j, err;

    for (i = 0; i < size; i++) {
        for (j = 0; j < CLUSTER; j++) {
            err = write(pipe[WRITE], msg[i][j], MAXLEN);
            if (err != 0)
            {
                ret = err;
            }
        }
    }
    close(pipe[WRITE]);
    return ret;
}

string **readAndWaitN(int pipe[], pid_t son, int size) {

    close(pipe[WRITE]);
    string **msg;
    msg = malloc(size * sizeof(string *));
    int i, j;
    for (i = 0; i < size; ++i) {
        msg[i] = malloc(CLUSTER * sizeof(char *));
    }
    int rd;
    int err = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < CLUSTER; j++) {
            msg[i][j] = malloc(MAXLEN);
            rd = read(pipe[READ], msg[i][j], MAXLEN);
            if (rd == -1)
            {
                //printf("%d\n", rd);
                err = rd;
            }
            msg[i][j][rd] = 0;
        }
    }
    //printf("(read)ERR=%d",err);
    close(pipe[READ]);
    waitpid(son, NULL, 0);
    return msg;
}

void storeOnMap(map fileData[], int **values, int size, int index) {
    int i, j, k = 0;
    for (i = index; i < (index + size); ++i) {
        //printf("\tsono arrivato a %d\n", index + i + 1);
        //printf("\tstatistiche di %s\n", fileData[i]->name);
        for (j = 0; j < CLUSTER; ++j) {
            fileData[i]->stats[j] += values[k][j];
            //printf("\t\t%d\n", fileData[i]->stats[j]);
        }
        ++k;
    }
}

unsigned long computeHash(string fname, int dim, BOOL compare) {
    int i = 0;
    int err = 0;
    unsigned long hash = 0;
    int digits = 10;

    if (compare == TRUE) {
        dim = fileDim(fname);
    }
    string content = malloc(dim);
    err = readFile(fname, content, 0, dim);
    //content[dim] = 0;
    //printf("%s", content);
    while (content[i] != '\0') {
        if (i % 2 == 0) {
            hash += ((content[i] + 100) % 128);
        } else if (i % 3 == 0) {
            if(content[i] == content[i - 1]) {
                hash += 69 * dim;
            }
        } else {
            hash += content[i];
        }
        //printf("%c", content[i]);
        //printf("%d", i);
        ++i;
    }
    free(content);
    hash += 2000;
    int current = countDigits(hash);

    /*
    for (i = 0; i < digits; ++i) {
        sprintf(fileData->fileHash[i], "%d", hash * 123456);
    }*/

    while (current != digits) {
        current = countDigits(hash);
        //printf("%d\n", current);
        if (current < digits) {
            hash *= 123456;
        } else  if (current > digits) {
            hash /= 9876;
        }
    }
    //printf("\n");
    //printf("%d\n", countDigits(hash));
    //fileData->fileHash = hash;
    return hash;
}

int countDigits(unsigned long n) {
    int count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}

int fileDim(string file) {
    int ret = 0;
    int fp;
    fp = open(file, O_RDONLY);
    ret = lseek(fp, 0, SEEK_END);
    close(fp);
    return ret;
}

//------------------------ process function section-----------------------


/**
 * Logica del processo P
 * DA AGGIORNAREs
 * @param c_son       pid del processo figlio
 * @param pipe_c      pipe di comunicazione tra C e P
 * @param pipe_q      pipe di comunicazione tra P e Q
 * @param files       lista dei files da analizzare
 * @param N           numero dei processi Q
 * @param M           numero dei processi P
 * @param total       il numero totale dei file da analizzare
 * @param fileIndex   valore che conserva la posizione del prossimo file da analizzare
 * @param part        non mi ricordo cos'è ma bisogna passarlo a Q
 * @param fdim        non mi ricordo cos'è ma bisogna passarlo a Q
 * @param index_p     numero del processo P corrente
 * @param file_per_p  numero di file che deve analizzare questo processo P
 * @param fileData TODO
 */
int processP(pid_t c_son, int pipe_c[][2], int pipe_q[][2], string *file_P,
             int N, int M, int total, int fileIndex, int *part, int *fdim, 
             int index_p,int file_per_p, int f_Psize) {
    //creo pipe fra C e P
    int l, g;
    int return_value;
    if (c_son == 0) {
        //processo P
        string **qTop[M];
        int **dataCollected = malloc(f_Psize * sizeof(int *));
        for (l = 0; l < f_Psize; l++) {
            dataCollected[l] = malloc(CLUSTER * sizeof(int));
            for (g = 0; g < CLUSTER; g++) {
                dataCollected[l][g] = 0;
            }
        }
            
        printf("P created pid=%d ppid=%d\n", getpid(), getppid());                
        /*int k = 0;
        string file_P[file_per_p];
        int f_Psize = 0;
        int fileIndexTemp = fileIndex;
        while (k < file_per_p) {
            if (fileIndexTemp < total) {
                file_P[k] = files[fileIndexTemp++];
                f_Psize++;
                //printf("sono il P%d e ho preso il file  numero %d\n con %d file per p\n", index_p, fileIndexTemp - 1, file_per_p);
            }
            if (fileIndexTemp - 1 == total) {
                file_P[k] = 0;
                //printf("ho finito i file\n");
                }
            ++k;
        }*/
        int j;
        //creo M processi di tipo Q
        for (j = 0; j < M; j++) {
            //creo la pipe fra P e Q
            pipe(pipe_q[j]);
            int p_son = fork();
            if (p_son == -1) {
                printf("error occurred at line 46\n");
                return_value = 46;
            } else {
                if (p_son == 0) {
                    return_value = processQ(part, fdim, file_P, f_Psize, 
                                            j, fileIndex, M, pipe_q[j]);
                    exit(0);
                } else {
                    //successive parti del processo P
                    qTop[j] = readAndWaitN(pipe_q[j], p_son, f_Psize);
                    //printf("\t%d\n", f_Psize);
                    int **tmp = getValuesFromStringN(qTop[j], f_Psize);
                    for (l = 0; l < f_Psize; l++) {
                        for (g = 0; g < CLUSTER; g++) {
                            dataCollected[l][g] += tmp[l][g];
                        }
                    }
                    for (g = 0; g < f_Psize; g++) {
                        free(tmp[g]);
                    }
                    //free(tmp);//new: tmp non ci serve più perchè il suoi valori vengono passati a dataCollected
                }
            }                   
        }/*
        for (l = 0; l < f_Psize; l++) {
            for (g = 0; g < CLUSTER; ++g) {
                printf("\t\t%d\n", dataCollected[l][g]);
            }
        }*/
        return_value = writePipeN(pipe_c[index_p], statsToStringN(dataCollected, f_Psize), f_Psize);
        for (g = 0; g < f_Psize; g++) {
            free(dataCollected[g]);
        }
        return return_value;
    }
}
/**
 * @param range     vedi documentazione processoQ_n
 * @param dims         "                      "
 * @param fname        "                      "
 * @param f_Psize      "                      "
 * @param q_loop       "                      "
 * @param index        "                      "
 * @param m            "                      "
 * @param pipe_q    pipe di comunicazione tra P e Q
 */
int processQ(int *range, int *dims, char** fname, int f_Psize, 
             int q_loop, int index, int m, int pipe_q[]) {
    printf("\tQ created pid=%d ppid=%d\n", getpid(), getppid());
    int i;
    int** counter = processoQ_n(range, dims, fname, f_Psize,
                               q_loop, index, m);
    string **message = statsToStringN(counter, f_Psize);
    int err = writePipeN(pipe_q, message, f_Psize);
    for (i = 0; i < f_Psize; i++) {
        free(counter[i]);
    }
    //free(counter);//new: counter non ci serve più perchè il suo valore viene passato a message
    return err;
}

///////////funzioni di messaggistica
//da cambiare

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

void sender(int data[]) {
    string *message = statsToString(data);
    key_t key = ftok(PathName, ProjectId);
    if (key < 0) 
        report_and_exit("couldn't get key...");

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0) 
        report_and_exit("couldn't get queue id...");

    //char* payloads[] = {"msg1", "msg2", "msg3", "msg4", "msg5"};
    int types[CLUSTER];
    int i;
    for (i = 0; i < CLUSTER; i++) {
        types[i] = i + 1;
    }
    for (i = 0; i < CLUSTER; i++) {
        /* build the message */
        queuedMessage msg;
        msg.type = types[i];
        strcpy(msg.payload, message[i]);
        /* send the message */
        msgsnd(qid, &msg, sizeof(msg), IPC_NOWAIT); /* don't block */
        printf("%s sent as type %i\n", msg.payload, (int) msg.type);
    }
}




#endif