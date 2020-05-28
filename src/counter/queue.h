#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "counter.h"

#define ProjectId 123
#define PathName  "queue.h" /* any existing, accessible file would do */

