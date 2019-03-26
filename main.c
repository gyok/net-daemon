#include <stdio.h>
#include "daemonization.h"

#define DELAY 30

int main() {
    daemonize();

    while (1) {
        printf("OK");
        sleep(DELAY);
    }

    return 0;
}
