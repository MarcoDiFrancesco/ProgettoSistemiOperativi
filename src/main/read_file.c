#ifndef _MAIN_H_
#define _MAIN_H_

#include "main.h"

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

/**
 * Concatenate 3 strings in 1.
 */
char *concat(const char *s1, const char *s2, const char *s3) {
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);  // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

/**
 * Given a path of a directory it returns all files inside it.
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

/**
 * Removes new line character from a given string
 * 
 * Credits: https://stackoverflow.com/a/28462221/7924557
 */
void removeNewline(char *string) {
    string[strcspn(string, "\n")] = 0;
}

/**
 * Input a string of paths, separate on the spaces, and send
 * the path to Analyzer
 * 
 * Credits: https://stackoverflow.com/a/28462221/7924557
 */
void splitAndSendPaths(char *string) {
    // TODO: check if user inputs ,.-
    if (string[MAX_INPUT_LENGHT - 2] != '\0')
        printf("Warning: you succeeded the %d char limit, the last path might not be counted\n", MAX_INPUT_LENGHT);
    char *processPath = getProcessPath("/proc/self/exe");
    // TODO: free(processPath)
    printf("Process path: %s\n", processPath);

    /**  WHAT TO DO NEXT
     * Now split the path of the executable /root/bin/main in /root/bin/ and add analyzer to
     * get /root/bin/analyzer, than execute it, open the pipe in that side and get the passed
     * paths given below.
     */

    char *singlePath;                  // Contains the splited path, e.g. /root/test/file.txt
    singlePath = strtok(string, " ");  // Split in space
    while (singlePath != NULL) {
        sendPath(singlePath);
        singlePath = strtok(NULL, " ");
    }
}

/**
 * Input a string containing a path and send it to Analyzer
 */
void sendPath(char *string) {
    printf("I'm sending to Analyzer: '%s'...", string);
    fflush(stdout);

    int fd;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);
    write(fd, string, strlen(string) + 1);
    close(fd);
    printf("Done!\n");
}

char *getProcessPath(char *process) {
    char *buff = malloc(PATH_MAX);
    ssize_t len = readlink(process, buff, PATH_MAX - 1);
    if (len != -1) {
        buff[len] = '\0';
        return buff;
    }
    return "";
    // TODO: handle error condition
}

#endif