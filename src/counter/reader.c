#include "counter.h"

string *readerMessage();

int main(){
  int i;
  string *result = readerMessage();
  for(i=0;i<CLUSTER;i++){
    printf("%s ", result[i]);
  }
  printf("\n");
}

string *readerMessage() {
  string *ret=(char **)malloc(CLUSTER * sizeof(char *));
    int i;
    for (i = 0; i < CLUSTER; ++i) {
        ret[i] = (char *)malloc(12 * sizeof(int));
    }
  key_t key= ftok(PathName, ProjectId); /* key to identify the queue */
  if (key < 0) report_and_exit("key not gotten...");

  int qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
  if (qid < 0) report_and_exit("no access to queue...");

  int types[] = {1, 2, 3, 4, 5}; /* different than in sender */
  queuedMessage msg; /* defined in queue.h */
  for (i = 0; i < CLUSTER; i++) {
    if (msgrcv(qid, &msg, sizeof(msg), types[i], MSG_NOERROR | IPC_NOWAIT) < 0)   puts("msgrcv trouble...");
    printf("%s received as type %i\n", msg.payload, (int) msg.type);
    strcpy(ret[i], msg.payload);
  }
  /** remove the queue **/
  if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
    report_and_exit("trouble removing queue...");

  return ret;
}