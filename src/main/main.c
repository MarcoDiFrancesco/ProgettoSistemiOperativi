#include "../read_file/read_file.h"

// https://stackoverflow.com/a/35050382/7924557
int open_file(const char* dir) {
    int status;
    int pid = fork();

    if (pid == -1) {  // Error in forking
        return 1;
    } else if (pid == 0) {  // Child section
        execl("/bin/bash", "bash", "-c", dir, NULL);
        sleep(5);
    } else {  // Father section
        waitpid(pid, &status, 0);
        printf("pid=%d status=%d\n", pid, status);
    }
    return status;
}

int main(int argc, char** argv) {
    string report_path = "./bin/report";
    string analyzer_path = "./bin/analyzer";

    if (!file_exists(report_path)) {
        printf("File %s does not exist, run 'make build' to create it!\n", report_path);
        return 1;
    } else if (!file_exists(analyzer_path)) {
        printf("File %s does not exist, run 'make build' to create it!\n", analyzer_path);
        return 1;
    }
    printf("Analyzer return status: %d\n", open_file(analyzer_path));
    printf("Report return status: %d\n", open_file(report_path));

    int choice;
    while (1) {
        printf("1.Create Train\n");
        printf("2.Do domething else\n");
        printf("\nEnter Your choice : ");
        fflush(stdin);
        scanf("%d", &choice);
        // do something with choice
        // ...
        // ask for ENTER key
        printf("Press [Enter] key to continue.\n");
        while (getchar() != '\n')
            ;       // option TWO to clean stdin
        getchar();  // wait for ENTER
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
