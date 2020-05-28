#include "../read_file/read_file.h"

/*
 * Max size when reading from input.
 * Limitaion given by C99.
 * Credits: https://stackoverflow.com/a/11488687/7924557
*/
#define MAX_INPUT_LENGHT 4096

char *report_path[] = {"/root/bin/report", NULL};
char *analyzer_path[] = {"/root/bin/analyzer", NULL};

/*
 * This function runs the program specified on path, in a completely separated process.
 * The program is checked for existance
*/
int run_program(char **path) {
    if (!file_exists(path[0]))
        return 2;
    else if (!file_is_executable(path[0]))
        return 3;
    else if (is_folder(path[0]))
        return 4;
    else if (is_link(path[0]))
        return 5;

    int pid = fork();

    if (pid == -1) {  // Error in forking
        return 1;
    } else if (pid == 0) {  // Child section
        execvp(path[0], path);
    }
    return 0;
}

void get_analytics() {
    printf("This is the table with all the analytics: TODO\n");
}

int main() {
    int r;
    r = run_program(analyzer_path);

    if (r == 0)
        printf("Analyzer started\n");
    else if (r == 1)
        printf("Error in forking\n");  // TODO: do something about not being able to fork
    else if (r == 2)
        printf("File does not exist, building it\n");
    else if (r == 3)
        printf("File is not executable, rebuilding it\n");  // TODO: check for file integrity using an hash function
    else if (r == 4)
        printf("File is a directory, rebuilding\n");
    else if (r == 5)
        printf("File is soft link, rebuilding\n");

    if (r == 2 || r == 3 || r == 4 || r == 5)
        system("cd && make clean && make build");
    // TODO: check for input > 4096 and tell the user
    int choice;
    while (1) {
        printf("1. Add files to analyze\n");
        printf("2. Get file analytics\n");
        printf("Enter Your choice: ");
        fflush(stdin);
        scanf("%d", &choice);
        if (choice == 1) {
            char *s = malloc(MAX_INPUT_LENGHT);
            if (s == NULL) {
                printf("No memory available\n");  // TODO: test it
            }
            printf("Write something:\n");
            fgets(s, MAX_INPUT_LENGHT, stdin);
            fflush(stdin);
            printf("--- %s ---", s);
            free(s);
        } else if (choice == 2) {
            get_analytics();
        } else {
            printf("This is not a valid choice, type one of the following:\n");
        }
        while (getchar() != '\n')
            ;  // option TWO to clean stdin
        // getchar();  // wait for ENTER
    }
    return 0;
}

/*
    char *path = "bin/analyzer";
    if (file_exists(path))
        if (is_folder(path)) {
            node file_list = listFiles(path);
            while (file_list != NULL) {
                printf("%s", file_list->str);  // Just for demonstration
                file_list = file_list->next;
            }
        } else {
            printf("is a file\n");
        }
    else
        printf("no file nor folder found\n");
    return 0;
*/
