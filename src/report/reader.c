#include "queue.h"
#include "counter.h"

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

int main() {
  key_t key= ftok(PathName, ProjectId); /* key to identify the queue */
  if (key < 0) report_and_exit("key not gotten...");

  int qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
  if (qid < 0) report_and_exit("no access to queue...");

  int types[CLUSTER];
  int i;
  for(i=0; i<CLUSTER; i++){
      types[i] = i;
  } 
  int i;
  for (i = 0; i < CLUSTER; i++) {
    queuedMessage msg; /* defined in queue.h */
    if (msgrcv(qid, &msg, sizeof(msg), types[i], MSG_NOERROR | IPC_NOWAIT) < 0)
      puts("msgrcv trouble...");
    printf("%s received as type %i\n", msg.payload, (int) msg.type);
  }

  /** remove the queue **/
  if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
    report_and_exit("trouble removing queue...");
  return 0;
}S