#include "result.h"

int main(int argc, char *argv[]) {
    int *stats = malloc(CLUSTER*sizeof(int));

    stats[0] = 12;
    stats[1] = 5;
    stats[2] = 3;
    stats[3] = 0;
    stats[4] = 1;

    string *str = statsToString(stats);
    read_result(5, str);

    free(stats);
    free(str);
    
    return 0;
}
