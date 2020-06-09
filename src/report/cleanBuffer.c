#include "report.h"

int main(){
    key_t key= ftok(PathName, ProjectId); /* key to identify the queue */
    if (key < 0) report_and_exit("key not gotten...");

    int qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
    if (qid < 0) report_and_exit("no access to queue...");

    if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");

    key= ftok(PathName, 3); /* key to identify the queue */
    if (key < 0) report_and_exit("key not gotten...");

    qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
    if (qid < 0) report_and_exit("no access to queue...");

    if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");

    key= ftok(PathName, 4); /* key to identify the queue */
    if (key < 0) report_and_exit("key not gotten...");

    qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
    if (qid < 0) report_and_exit("no access to queue...");

    if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");

    return 0;
}
