#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define ProjectId 123
#define PathName  "../tmp/msg.h" /* any existing, accessible file would do */
#define MsgLen    4
#define MsgCount  6

typedef struct {
  long type;                 /* must be of type long */
  char payload[MsgLen + 1];  /* bytes in the message */
} queuedMessage;