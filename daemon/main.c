#include <stdio.h>
#include "daemonization.h"
#include "ndsrv.h"

#define DELAY 30

int mainCycle() {
    // while (1) {
    //     printf("OK");
    //     sleep(DELAY);
    // }
    return 0;
}

void netDaemon() {
    daemonize();
    initNdSrv();
}


int main() {
    netDaemon();

    return 0;
}
