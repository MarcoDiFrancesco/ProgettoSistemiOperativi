#include "report.h"

int main(int argc, char *argv[]) {
    signal(SIGUSR2, sighandler);
    int numFile;
    map read = readerMessage(&numFile);
    read_result(read, numFile);
    return 0;
}
