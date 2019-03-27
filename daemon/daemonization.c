#include "daemonization.h"

void handleSignals() {
    signal(SIGCHLD, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
}


int daemonize() {
    printf("daemonize\n");
    pid_t pid; 

    if (getppid() == 1) { return 0; } // already daemon
    
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "error: failed fork\n");
        exit(EXIT_FAILURE); // fork error
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS); // for parent
    } 
    if (setsid() < 0) {
        fprintf(stderr, "error: failed setsid\n");
        exit(EXIT_FAILURE);
    }

    handleSignals();
    for (int i = getdtablesize(); i>=0; --i) {
        close(i);
    }
    umask(027);
    chdir("/tmp");

    return 0;
}
