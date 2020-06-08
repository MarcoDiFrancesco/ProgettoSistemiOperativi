#include "analyzer.h"
//#include "counter/counter.h"

BOOL isTxt(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 5] == '.' && file[length - 4] == 't' && file[length - 3] == 'x' && file[length - 2] == 't') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 4] == '.' && file[length - 3] == 't' &&
        file[length - 2] == 'x' && file[length - 1] == 't') {
        ret = TRUE;
    }
    return ret;
}

BOOL isCpp(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 5] == '.' && file[length - 4] == 'c' && file[length - 3] == 'p' && file[length - 2] == 'p') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 4] == '.' && file[length - 3] == 'c' &&
        file[length - 2] == 'p' && file[length - 1] == 'p') {
        ret = TRUE;
    }
    return ret;
}

BOOL isC(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 3] == '.' && file[length - 2] == 'c') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 2] == '.' && file[length - 1] == 'c') {
        ret = TRUE;
    }
    return ret;
}

BOOL isJava(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' &&
        file[length - 6] == '.' && file[length - 5] == 'j' &&
        file[length - 4] == 'a' && file[length - 3] == 'v' &&
        file[length - 2] == 'a') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 5] == '.' && file[length - 4] == 'j' &&
        file[length - 3] == 'a' && file[length - 2] == 'v' &&
        file[length - 1] == 'a') {
        ret = TRUE;
    }
    return ret;
}

BOOL isPy(string file, int length) {
    BOOL ret = FALSE;
    if (file[length - 1] == '\n' && file[length - 4] == '.' && file[length - 3] == 'p' && file[length - 2] == 'y') {
        file[length - 1] = '\0';
        return TRUE;
    }
    if (file[length - 3] == '.' &&
        file[length - 2] == 'p' &&
        file[length - 1] == 'y') {
        ret = TRUE;
    }
    return ret;
}

//funzioni per cartelle

node createNode() {
    node temp;
    temp = (node)malloc(sizeof(struct LinkedList));
    temp->next = NULL;
    return temp;
}

node addNode(node head, char *new_str) {
    char *dest;  // Create new str
    dest = malloc(sizeof(char) * (strlen(new_str) + 1));
    strcpy(dest, new_str);  // Copy string into a new string

    node temp, p;
    temp = createNode();

    temp->str = dest;  // Pointer to new string
    if (head == NULL) {
        head = temp;
    } else {
        p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}

// https://stackoverflow.com/a/13098645/7924557
int file_is_executable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR);
}

// https://stackoverflow.com/a/230070/7924557
int file_exists(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0);
}

// https://stackoverflow.com/a/4553053/7924557
int is_folder(char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0)
        return 0;
    return S_ISDIR(sb.st_mode);
}

// https://stackoverflow.com/a/3985085/7924557
int is_link(char *path) {
    struct stat sb;
    int x;
    x = lstat(path, &sb);
    return S_ISLNK(sb.st_mode);
}

/*
 * Concatenate 3 strings in 1.
*/
char *concat(const char *s1, const char *s2, const char *s3) {
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);  // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

/*
 * Ginven a path of a directory it returns all files inside it.
*/
node listFiles(char *path) {
    node files_list = createNode();  // Tail of the list
    node head = files_list;
    FILE *fp;

    char *command = concat("find ", path, " -type f");
    fp = popen(command, "r");  // Open pointer to command output
    if (fp == NULL) {
        printf("Failed to run command");
        return files_list;
    }

    char p[1000];  // Max lenght of the path
    while (fgets(p, sizeof(p), fp) != NULL) {
        head = addNode(head, p);
    }

    pclose(fp);               // Close pointer to command output
    return files_list->next;  // First node is NULL, so return second
}

BOOL containsDot(string str) {
    int len = strlen(str);
    int i;
    BOOL ret = FALSE;
    for (i = len - 1; len >= 0 && str[i] != '/'; i--) {
        if (str[i] == '.')
            ret = TRUE;
    }
    return ret;
}

void sendMessage(int *pipe, string message){
    close(pipe[READ]);
    write(pipe[WRITE], message ,strlen(message));
    close(pipe[WRITE]);
}

string addThingsToCounter(){
    printf("\nL'analisi dei file inseriti e' stata avviata...\nVuoi modificare i file? [f]\nVuoi modificare N o M? [n]\nNon modificare nulla (non potrai più modificare nulla in seguito)[x]\n");
        char input;
        scanf(" %c", &input);
        while(input != 'f' && input != 'n' && input != 'x'){
            printf("Inserisci solamente [f] o [n] o [x]\n");
            fflush(stdin);
            fflush(stdout);
            scanf(" %c", &input);
        }
        switch(input){
            case 'f': return addFile();
            case 'n': return changeNM();
            case 'x': return "X";
        }
}

string addFile(){
    string input = malloc(MAXLEN);
    string ret = malloc(MAXLEN +3);
    strcat (ret, "-f ");
    BOOL space = TRUE;
    printf("Inserisci il file con il suo percorso\n");
    while(space==TRUE){
        fflush(stdin);
        fflush(stdout);
        fgets(input, MAXLEN, stdin);
        int i;
        space = FALSE;
        for(i=0; i<strlen(input); i++){
            if(input[i] == ' '){
                space = TRUE;
                printf("Forse hai inserito due file\nInseriscine solo uno\n");
            }
        }
    }
    strcat(ret, input);
    return ret; 
}

string changeNM(){
    char input;
    string ret = malloc(10);
    strcat(ret, "-");
    int input_int;
    printf("Vuoi cambiare N o M? [n]/[m]\n");
    scanf(" %c", &input);
    while(input != 'n' && input != 'm'){
        printf("Inserisci solamente [n] o [m]\n");
        fflush(stdin);
        fflush(stdout);
        scanf(" %c", &input);
    }
    string tmp;
    switch(input){
        case 'n':
            strcat(ret, "n ");
            printf("inserisci n: ");
            scanf(" %d", &input_int);
            tmp = malloc(7);
            sprintf(tmp, "%d", input_int);
            strcat(ret, tmp);
            return ret;
        case 'm':
            strcat(ret, "m ");
            printf("inserisci m: ");
            scanf(" %d", &input_int);
            tmp = malloc(7);
            sprintf(tmp, "%d", input_int);
            strcat(ret, tmp);
            return ret;      
    }
}

string readPipe(int pipe[]) {
    close(pipe[WRITE]);
    string msg;
    msg = malloc(MAXLEN);
    int i;
    int rd;
    rd=read(pipe[READ], msg, MAXLEN);
    msg[rd] = 0;
    close(pipe[READ]);
    return msg;
}

void dynamicReadAnalyzer(int *pipe){
    
}

void counter(int nArgs, string *argomenti, int *controlPipe){
    
    printf("Counter started\n");
    if (nArgs < 4) {
        printf("Counter chiamato con argomenti insufficienti\n");
        return ;
    }
    signal(SIGUSR2, sighandlerP);
    int k, b = 0;
    N = atoi(argomenti[1]);
    M = atoi(argomenti[2]);
    PIds = malloc(N * sizeof(pid_t));
    QIds = malloc(N * sizeof(pid_t *));
    for (k = 0; k < N; ++k) {
        PIds[k] = 0;
        QIds[k] = malloc(M * sizeof(pid_t));
        for (b = 0; b < M; ++b) {
            QIds[k][b] = 0;
        }
    }
    boolQ = malloc(sizeof(BOOL) * N);
    for (k = 0; k < N; ++k) {
        boolQ[k] = FALSE;
    }
    boolP = FALSE;

    checkQ = malloc(sizeof(int) * N);
    for (k = 0; k < N; k++) {
        checkQ[k] = 0;
    }
    checkP = 0;

    int fileTotal = atoi(argomenti[3]);

    string files[fileTotal];
    int j;
    for (j = 0; j < fileTotal; j++) {
        files[j] = malloc(sizeof(char) * 100);
        strcpy(files[j], argomenti[j + 4]);
    }
    int fileIndex = 0;
    int file_restanti = fileTotal;
    int file_per_p = ceiling(fileTotal, N);
    int return_value;
    //pipes
    int p_c[N][2];
    int q_p[M][2];
    //counters
    //int data[CLUSTER];
    int g;
    int *part = filesPart(files, fileTotal, M);
    int *f_dim = filesDim(files, fileTotal);
    int *f_Psize = malloc(N * sizeof(int));
    for (k = 0; k < N; ++k) {
        f_Psize[k] = 0;
    }
    printf("Ciao merda sono qui 1\n");
    map fileData = malloc(fileTotal * sizeof(FileMap));
    printf("Ciao merda sono qui 2\n");
    int i;
    for (i = 0; i < fileTotal; i++) {
        fileData[i].name = files[i];
        printf("Ciao index %d\n",i);
        fileData[i].fileHash = computeHash(files[i], f_dim[i], FALSE);
        printf("Ciao merda index %d\n",i);
    }
    printf("\n\nProcess C pid=%d\n",getpid());
    for (i = 0; i < N; i++) {
        int k = 0;
        string file_P[file_per_p];
        int fileIndexTemp = fileIndex;
        while (k < file_per_p) {
            if (fileIndexTemp < fileTotal) {
                file_P[k] = files[fileIndexTemp++];
                f_Psize[i]++;
            }
            if (fileIndexTemp - 1 == fileTotal) {
                file_P[k] = 0;
            }
            ++k;
        }

        pipe(p_c[i]);
        PIds[i] = fork();
        printf("qui pids[%d]=%d e N=%d\n", i, PIds[i], N);
        if (PIds[i] == -1) {
            printf("error occurred at line 35\n");
            return_value = 35;
        } else {
            if (PIds[i] == 0) {
                PIds[i] = getpid();
                return_value = processP(PIds[i], p_c, q_p, file_P, N, M,
                                        fileTotal, fileIndex, part, f_dim,
                                        i, file_per_p, f_Psize[i]);
                if (kill(getppid(), SIGUSR2) == 0) {
                    printf("P ha mandato una signal qid : %d\n", getpid());
                } else {
                    printf("!!!!Perror  %s\n", strerror(errno));
                }

                exit(return_value);
            } else {
                //successive parti del processo C
            }
        }
        fileIndex += file_per_p;
        file_restanti -= file_per_p;
        if (i != N - 1) {
            file_per_p = ceiling(file_restanti, N - i - 1);
        }
    }

    dynamicReadAnalyzer(controlPipe);

    printf("Sto analizzando...");
    while (boolP == FALSE) {
        system("sleep 1");
        printf(".");
        fflush(stdout);
        int tmp_counter = 0;
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                if (waitpid(QIds[i][j], NULL, WNOHANG) == QIds[i][j]) {
                    tmp_counter++;
                }
            }
        }
        if (tmp_counter == N * M)
            boolP = TRUE;
    }

    fileIndex = 0;
    for (i = 0; i < N; i++) {
        string **buffer = readAndWaitN(p_c[i], f_Psize[i]);
        int **temp = getValuesFromStringN(buffer, f_Psize[i]);
        storeOnMap(fileData, temp, f_Psize[i], fileIndex);
        free(temp);
        fileIndex += f_Psize[i];
    }
    free(f_Psize);

    //printf("Printing data....\n");
    //printf("\nHo analizzato i seguenti files:\n\n");
    for (i = 0; i < fileTotal; ++i) {
        //printf("%s\n", fileData[i].name);
        if (fileData[i].fileHash != computeHash(files[i], f_dim[i], TRUE)) {
            printf("WARNING: %s has been modified while being analyzed,\nthis data is about the file prior to modification\n", fileData[i].name);
        }
        /*printf("Old hash= %lu, new hash= %lu\n", fileData[i].fileHash, computeHash(files[i], f_dim[i], TRUE));
        printf("\tNumero di lettere maiuscole calcolato= %d\n", fileData[i].stats[UPPERCASE]);
        printf("\tNumero di lettere minuscole calcolato= %d\n", fileData[i].stats[LOWERCASE]);
        printf("\tNumero di numeri calcolato= %d\n", fileData[i].stats[NUMBERS]);
        printf("\tNumero di spazi calcolato= %d\n", fileData[i].stats[SPACES]);
        printf("\tNumero di punteggiatura calcolato= %d\n", fileData[i].stats[PUNCTUATION]);
        printf("\tNumero di parentesi= %d\n", fileData[i].stats[PARENTHESIS]);
        printf("\tNumero di operatori matematici= %d\n", fileData[i].stats[MATH_OPERATORS]);
        printf("\tNumero di altro calcolato= %d\n", fileData[i].stats[OTHER]); */
    }

    printf("\n\n");
    printf("Numero di file analizzati: %d\n", fileTotal);
    printf("Per inviare i dati è necessario lanciare il report [bin/report]\n");
    sender(fileData, fileTotal);
    printf("Dati inviati con successo!\n");

    //libero spazio in memoria
    free(part);
    free(f_dim);

    return ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL cantWrite = FALSE;

int writePipe(int pipe[], string *msg) {
    int ret = 0;  //per eventuali errori
    close(pipe[READ]);
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
int readFile(char *filename, char *filedata, int start, int stop) {
    int sk, rd;
    int ret;
    int file = open(filename, O_RDONLY);
    if (file >= 0) {
        sk = lseek(file, start * sizeof(char), SEEK_SET);
        if (sk != -1) {
            rd = read(file, filedata, stop - start);
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
void countLetters(int dim, char *s, int *counter) {
    int i = dim - 1;
    for (i; i >= 0; i--) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            //lettera maiuscola
            counter[UPPERCASE]++;
        } else if (s[i] >= 'a' && s[i] <= 'z') {
            //lettera minuscola
            counter[LOWERCASE]++;
        } else if (s[i] >= '0' && s[i] <= '9') {
            //numero
            counter[NUMBERS]++;
        } else if (s[i] == ' ') {
            //spazi
            counter[SPACES]++;
        } else if (s[i] == '.' || s[i] == ',' || s[i] == ':' ||
                   s[i] == ';' || s[i] == '?' || s[i] == '!') {
            //punteggiatura
            counter[PUNCTUATION]++;
        } else if (s[i] == '(' || s[i] == ')' || s[i] == '[' ||
                   s[i] == ']' || s[i] == '{' || s[i] == '}') {
            //parentesi
            counter[PARENTHESIS]++;
        } else if (s[i] == '+' || s[i] == '-' || s[i] == '*' ||
                   s[i] == '/' || s[i] == '%' || s[i] == '^' ||
                   s[i] == '<' || s[i] == '>' || s[i] == '=') {
            //simboli matematici
            counter[MATH_OPERATORS]++;
        } else {
            counter[OTHER]++;
        }
    }
}

int *processoQ(int from, int to, char *fname) {
    char *testo;
    int *stats;
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

    if (i == 0)
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
int *processoQ_n(int *range, int *dims, char **fname, int n, int q_loop, int index, int M) {
    char *testo;
    int *stats;
    int i, j, k, alloc_value;
    int inizio[n], fine[n];
    i = 0;
    // Recupero degli indici di inizio e fine, per ciascuno degli n file:
    // si parte da index e si itera n volte.
    for (j = index; j < (index + n); j++) {
        // Controllo per gestire i casi in cui la dimensione del file non
        // è un multiplo di M.
        inizio[i] = range[j] * q_loop;
        if ((range[j] * (q_loop + 1) <= dims[j])) {
            fine[i] = range[j] * (q_loop + 1);
        } else {
            fine[i] = dims[j];
        }

        // Controllo per i file di dimensione inferiore a M - 1.
        if (inizio[i] > fine[i]) {
            inizio[i] = fine[i] = 0;
        }
        ++i;
    }
    stats = malloc(CLUSTER * sizeof(int));
    i = 0;

    for (i; i < CLUSTER; i++) {
        stats[i] = 0;
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
        countLetters(fine[j] - inizio[j], testo, stats);
        free(testo);
        if (i < 0) {
            break;
        }
    }

    if (i == 0)
        return stats;
    else
        return (int *)-1;
}

int **processoQ_n_new(int *range, int *dims, char **fname, int n, int q_loop, int index, int M) {
    char *testo;
    int **stats;
    int i, j, k, alloc_value;
    int inizio[n], fine[n];
    i = 0;

    /*for (i = 0; i < n; i++) {
        printf("\tsto analizzando = %s\n", fname[i]);
    }*/
    i = 0;

    // Recupero degli indici di inizio e fine, per ciascuno degli n file:
    // si parte da index e si itera n volte.
    for (j = index; j < (index + n); j++) {
        // Controllo per gestire i casi in cui la dimensione del file non
        // è un multiplo di M.
        inizio[i] = range[j] * q_loop;
        if ((range[j] * (q_loop + 1) <= dims[j])) {
            // printf("%d",range[j]*q_loop + j);
            fine[i] = range[j] * (q_loop + 1);
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

    stats = (int **)malloc(n * sizeof(int *));

    for (i = 0; i < n; i++) {
        stats[i] = malloc(CLUSTER * sizeof(int));
        for (j = 0; j < CLUSTER; ++j) {
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
        countLetters(fine[j] - inizio[j], testo, stats[j]);
        free(testo);
        if (i < 0) {
            break;
        }
    }
    if (i == 0)
        return stats;
    else
        return (int **)-1;
}

//---------------------------phil functions------------------------------------

/**
 * Funzione per trasformare un vettore di interi in una stringa che contenga
 * gli stessi valori.
 * @param values Vettori di interi da convertire.
 * @return La stringa che contiene gli stessi valori.
 */
char **statsToString(int *values) {
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

char ***statsToStringN(int **values, int size) {
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

int **getValuesFromStringN(char ***str, int size) {
    int i, j;
    int **values = (int **)malloc(size * sizeof(int *));
    for (i = 0; i < size; ++i) {
        values[i] = (int *)malloc(CLUSTER * sizeof(int));
        for (j = 0; j < CLUSTER; ++j) {
            values[i][j] = atoi(str[i][j]);
        }
        ////nl();
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
    int *ret = malloc(num * sizeof(int));
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
int ceiling(int first, int second) {
    int result = 0;
    if (first % second == 0) {
        result = first / second;
    } else {
        result = (first / second) + 1;
    }
    return result;
}

int writePipeN(int pipe[], string **msg, int size) {
    int ret = 0;  //per eventuali errori
    close(pipe[READ]);
    int i, j, err;

    for (i = 0; i < size; i++) {
        for (j = 0; j < CLUSTER; j++) {
            err = write(pipe[WRITE], msg[i][j], MAXLEN);
            if (err != 0) {
                ret = err;
            }
        }
    }
    close(pipe[WRITE]);
    return ret;
}

string **readAndWaitN(int pipe[], int size) {
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
            if (rd == -1) {
                err = rd;
            }
            msg[i][j][rd] = 0;
        }
    }
    close(pipe[READ]);
    return msg;
}

void storeOnMap(map fileData, int **values, int size, int index) {
    int i, j, k = 0;
    for (i = index; i < (index + size); ++i) {
        for (j = 0; j < CLUSTER; ++j) {
            fileData[i].stats[j] += values[k][j];
        }
        ++k;
    }
}

unsigned long computeHash(string fname, int dim, BOOL compare) {
    int i,
        err = 0;
    unsigned long hash = 0;
    int digits = 10;

    if (compare == TRUE) {
        dim = fileDim(fname);
    }
    string content = malloc(dim);
    err = readFile(fname, content, 0, dim);
    for (i = 0; i < dim; ++i) {
        if (i % 2 == 0) {
            hash += ((content[i] + 100) % 128);
        } else if (i % 3 == 0) {
            if (content[i] == content[i - 1]) {
                hash += 69 * dim;
            }
        } else {
            hash += content[i];
        }
    }
    hash+=2000;
    free(content);
    int current = countDigits(hash);

    while (current != digits) {
        current = countDigits(hash);
        if (current < digits) {
            hash *= 123456;
        } else if (current > digits) {
            hash /= 9876;
        }
    }
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

void nl() {
    printf("\n");
}

//------------------------ process function section-----------------------

/**
 * Logica del processo P
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
 */
int processP(pid_t c_son, int pipe_c[][2], int pipe_q[][2], string *file_P,
             int N, int M, int total, int fileIndex, int *part, int *fdim,
             int index_p, int file_per_p, int f_Psize) {
    signal(SIGUSR2, sigHandlerQ);
    //creo pipe fra C e P
    int l, g, j;
    int return_value;
    //processo P
    string **qTop[M];
    int **dataCollected = malloc(f_Psize * sizeof(int *));
    for (l = 0; l < f_Psize; l++) {
        dataCollected[l] = malloc(CLUSTER * sizeof(int));
        for (g = 0; g < CLUSTER; g++) {
            dataCollected[l][g] = 0;
        }
    }
    //printf("P created pid=%d ppid=%d\n", getpid(), getppid());
    //creo M processi di tipo Q
    for (j = 0; j < M; j++) {
        //creo la pipe fra P e Q
        pipe(pipe_q[j]);
        QIds[index_p][j] = fork();
        if (QIds[index_p][j] == -1) {
            printf("error occurred at line 46\n");
            return_value = 46;
        } else {
            if (QIds[index_p][j] == 0) {
                QIds[index_p][j] = getpid();
                //printf("pid processo Q: %d vs quello che abbiamo %d\n", getpid(), QIds[index_p][j]);
                return_value = processQ(part, fdim, file_P, f_Psize, j, fileIndex, M, pipe_q[j]);
                if (kill(getppid(), SIGUSR2) == 0) {
                    //printf("Q ha mandato una signal qid : %d\n", getpid());
                } else {
                    //printf("!!!!Qerror  %s\n", strerror(errno));
                }
                exit(return_value);
            } else {
                //successive parti del processo P
            }
        }
    }
    while (boolQ[index_p] == FALSE) {
        system("sleep 1");
        printf(".");
        fflush(stdout);
        int tmp_count = 0;
        int i, index;
        for (j = 0; j < M; j++) {
            if (waitpid(QIds[index_p][j], NULL, WNOHANG) == QIds[index_p][j]) {
                tmp_count++;
            }
        }
        if (tmp_count == M)
            boolQ[index_p] = TRUE;
    }

    for (j = 0; j < M; j++) {
        qTop[j] = readAndWaitN(pipe_q[j], f_Psize);
        int **tmp = getValuesFromStringN(qTop[j], f_Psize);
        for (l = 0; l < f_Psize; l++) {
            for (g = 0; g < CLUSTER; g++) {
                dataCollected[l][g] += tmp[l][g];
            }
        }
    }
    return_value = writePipeN(pipe_c[index_p], statsToStringN(dataCollected, f_Psize), f_Psize);
    for (g = 0; g < f_Psize; g++) {
        free(dataCollected[g]);
    }
    return return_value;
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
int processQ(int *range, int *dims, char **fname, int f_Psize,
             int q_loop, int index, int m, int pipe_q[]) {
    //printf("\tQ created pid=%d ppid=%d\n", getpid(), getppid());
    int i, j;
    int **counter = processoQ_n_new(range, dims, fname, f_Psize,
                                    q_loop, index, m);
    string **message = statsToStringN(counter, f_Psize);
    int err = writePipeN(pipe_q, message, f_Psize);
    for (i = 0; i < f_Psize; ++i) {
        free(counter[i]);
        free(message[i]);
    }
    //free(counter);//new: counter non ci serve più perchè il suo valore viene passato a message
    //free(message);
    return err;
}

///////////funzioni di messaggistica

void report_and_exit(const char *msg) {
    perror(msg);
    exit(-1); /* EXIT_FAILURE */
}

void sender(map data, int mapDim) {
    //signal handler
    signal(SIGUSR1, signalhandler);

    key_t key = ftok(PathName, ProjectId);
    if (key < 0) {
        printf("err: %s\n", strerror(errno));
        report_and_exit("couldn't get key...");
    }

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0)
        report_and_exit("couldn't get queue id...");

    int i;
    queuedMessage msgNum;
    sprintf(msgNum.payload, "%d", mapDim);
    msgNum.type = 1;
    msgsnd(qid, &msgNum, sizeof(msgNum), IPC_NOWAIT);
    int j, cont = 2;
    for (j = 0; j < mapDim; j++) {
        //invio nome file
        queuedMessage msgName;
        strcpy(msgName.payload, data[j].name);
        msgName.type = cont;
        int msgError = 0;
        msgError = msgsnd(qid, &msgName, strlen(msgName.payload) + 1, MSG_NOERROR | IPC_NOWAIT);

        if (msgError < 0) {
            cantWrite = TRUE;
            printf("Limite di dati inviati raggiunto. Aspetta che il report li legga...\n");
            while (cantWrite == TRUE) {
                //system("sleep 2");
                msgError = msgsnd(qid, &msgName, strlen(msgName.payload) + 1, MSG_NOERROR | IPC_NOWAIT);
                if (msgError == 0) cantWrite = FALSE;
            }
        }

        //conversione stats in message
        string *message = statsToString(data[j].stats);
        //invio dati file
        cont++;
        for (i = 0; i < CLUSTER; i++) {
                /* build the message */
                queuedMessage msg;
                msg.type = cont;
                strcpy(msg.payload, message[i]);
                /* send the message */
                msgError = msgsnd(qid, &msg, sizeof(msg), MSG_NOERROR); /* don't block */

                if(msgError<0){
                    cantWrite = TRUE;
                    printf("Limite di dati inviati raggiunto. Aspetta che il report li legga...\n");
                    while (cantWrite == TRUE) {
                        //system("sleep 2");
                        msgError = msgsnd(qid, &msgName, strlen(msgName.payload)+1, MSG_NOERROR | IPC_NOWAIT);
                        if (msgError == 0) cantWrite = FALSE;
                    }
                }
            }
            //printf("\terr (%d): %s\n", i, strerror(errno));
            cont++;
        }
}


void signalhandler(int sig) {
    printf("report ha letto i file in attesa di lettura\nOra analyzer puo ricominciare ad inviare\n");
    cantWrite = FALSE;
}

void sighandlerP(int sig) {
    checkP++;
    if (checkP == N) {
        boolP = TRUE;
    }
}

void sigHandlerQ(int sig) {
    int i;
    pid_t current_pid = getpid();
    //cerco l'indice del processo P
    for (i = 0; i < N; i++) {
        //printf("(%d) current pid=%d, PIDS[%d]=%d\n",i,current_pid,i,PIds[i]);
        if (current_pid == PIds[i])
            break;
    }
    checkQ[i]++;
    if (checkQ[i] == M) {
        //tutti i Q del processo P che gestisce la signal sono finiti ora si può leggere
        boolQ[i] = TRUE;
    }
}