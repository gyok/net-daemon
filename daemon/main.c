#include <stdio.h>
#include "daemonization.h"
#include "ndsrv.h"

void netDaemon() {
    // daemonize();
    initNdSrv();
}


int main() {
    netDaemon();

    return 0;
}
