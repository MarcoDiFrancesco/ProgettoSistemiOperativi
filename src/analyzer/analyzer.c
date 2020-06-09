#include "analyzer.h"

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

//funzioni messaggi per aggiungere roba

void report_and_exit(const char *msg) {
    perror(msg);
    exit(-1); /* EXIT_FAILURE */
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

    if (msgctl(qid, IPC_RMID, NULL) < 0) /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");

    string ret = malloc(strlen(msg.payload));
    strcpy(ret, msg.payload);

    return ret;
}

//funzioni per aggiungere roba al volo

string addThingsToCounter(){
    printf("\nL'analisi dei file inseriti e' stata avviata...\nVuoi modificare i file? [f]\nVuoi modificare N o M? [n]\nNon modificare nulla (non potrai più modificare nulla in seguito)[x]\n");
        char input;
        scanf(" %c", &input);
        while(input != 'f' && input != 'n' && input != 'x'){
            printf("Inserisci solamente [f] o [n] o [x]\n");
            __fpurge(stdin);
            __fpurge(stdout);
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
    strcpy (ret, "-f ");
    BOOL space = TRUE;
    printf("Inserisci il file con il suo percorso: ");
    while(space==TRUE){
        __fpurge(stdin);
        fgets(input, MAXLEN, stdin);
        int i;
        space = FALSE;
        for(i=0; i<strlen(input); i++){
            if(input[i] == ' '){
                space = TRUE;
                printf("Forse hai inserito due file\nInseriscine solo uno\n");
                break;
            }
        }
    }
    input[strlen(input) - 1] = '\0';
    strcat(ret, input);
    return ret; 
}

string changeNM(){
    char input,
         placeholder;
    string input_str = malloc(7);
    string ret = malloc(10);
    strcpy(ret, "-");
    int input_int,
        len;
    printf("Vuoi cambiare N o M? [n]/[m]\n");
    scanf(" %c", &input);
    while(input != 'n' && input != 'm'){
        printf("Inserisci solamente [n] o [m]\n");
        __fpurge(stdin);
        __fpurge(stdout);
        scanf(" %c", &input);
    }
    string tmp;
    __fpurge(stdin);
    switch(input){
        case 'n':
            strcat(ret, "n ");
            printf("inserisci n: ");
            __fpurge(stdin);
            fgets(input_str, len, stdin);
            while(stringIsInt(input_str) == FALSE){
                __fpurge(stdin);
                fgets(input_str, 7, stdin);
            }
            input_str[strlen(input_str) - 1] = '\0';
            strcat(ret, input_str);
            return ret;
        case 'm':
            strcat(ret, "m ");
            printf("inserisci m: ");
            __fpurge(stdin);
            fgets(input_str, len, stdin);
            while(stringIsInt(input_str) == FALSE){
                __fpurge(stdin);
                fgets(input_str, 7, stdin);
            }
            input_str[strlen(input_str) - 1] = '\0';
            strcat(ret, input_str);
            return ret;      
    }
}

/**
 * Check if string is a > 0 integer or not
 */
BOOL stringIsInt(char *str) {
    /*char *newString;
    long number = strtol(str, &newString, 10);  // Base 10

    if (*newString != '\0' || number < 1) {
        printf("Scrivi un numero maggiore di 0\n");
    } else {
        return TRUE;
    }
    return FALSE;*/
    int i,
        len = 0;

    BOOL ret = TRUE;
    
    len = strlen(str);
    //printf("strlen %d\n", len);
    for (i = 0; i < len - 1; ++i) {
        //printf("%c ", str[i]);
        if (isdigit(str[i]) == 0) {
            ret = FALSE;
            break;
        }
    }

    if (ret == FALSE || atoi(str) < 1) {
        printf("Scrivi un numero maggiore di 0\n");
    }

    __fpurge(stdin);
    return ret;

}

void sendSignal(int signal) {
    printf("Alanyzer sent sigusr1 to main\n");
    kill(getppid(), SIGUSR1);
}

void ignoreSignal(int signal) {
    printf("Signal ignored from analyzer\n");
}

//funzioni per pulizia

void clean(int msgKey, string path){
    key_t key= ftok(path, msgKey); /* key to identify the queue */
    if (key < 0) report_and_exit("key not gotten...");

    int qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
    if (qid < 0) report_and_exit("no access to queue...");

    if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");
}