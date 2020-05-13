#include "../read_file/read_file.h"

// https://stackoverflow.com/a/35050382/7924557
int open_file(const char *dir) {
    int pid, status;
    if (pid = fork()) {  // Parent section
        waitpid(pid, &status, 0);
    } else {  // Child section
        execl("/bin/bash", "bash", "-c", dir, NULL);
    }
    return status;
}

int main(int argc, char** argv) {
    // TODO: check for error -> "/usr/include/stdc-predef.h:1:0: fatal error: can't create precompiled header bin/main: Text file busy"

    printf("report: %d\n", open_file("./bin/report"));
    printf("analyzer: %d\n", open_file("./bin/analyzer"));
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
