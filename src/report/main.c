#include "report.h"

int main(int argc, char *argv[]) {
    //read_result(5, readerMessage());
    map test =readerMessage();
    printf("name : %s\n ", test[0].name);
    int i;
    for(i=0;i<CLUSTER;i++){
        printf("%d\t",test[0].stats[i]);
    }
    return 0;
}
