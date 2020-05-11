#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// https://www.hackerearth.com/practice/data-structures/linked-list/singly-linked-list/tutorial/
struct LinkedList {
    char *str;
    struct LinkedList *next;
};

typedef struct LinkedList *node;

node createNode();
node addNode(node head, char *new_str);
int file_exists(char *filename);
int is_folder(char *folder);
char *concat(const char *s1, const char *s2, const char *s3);
node listFiles(char *path);