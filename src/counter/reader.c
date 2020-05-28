#include "counter.h"

int main() {

  key_t key= ftok(PathName, ProjectId); /* key to identify the queue */
  if (key < 0) report_and_exit("key not gotten...");

  int qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
  if (qid < 0) report_and_exit("no access to queue...");

  int types[] = {1, 2, 3, 4, 5}; /* different than in sender */
  int i;
  queuedMessage msg; /* defined in queue.h */
  for (i = 0; i < CLUSTER; i++) {
    if (msgrcv(qid, &msg, sizeof(msg), types[i], MSG_NOERROR | IPC_NOWAIT) < 0)
      puts("msgrcv trouble...");
      printf("%s received as type %i\n", msg.payload, (int) msg.type);
  }
  /** remove the queue **/
  if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
    report_and_exit("trouble removing queue...");

  return 0;
}