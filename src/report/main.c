#include "report.h"

int main(int argc, char *argv[]) {
    signal(SIGUSR2, sighandler);
    sendConfirm("e", 7, "/root/src/main/main_functions.h");
    int numFile;
    map read = readerMessage(&numFile);
    read_result(read, numFile);
    return 0;
}
