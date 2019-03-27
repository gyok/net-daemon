#include "ndsrv.h"

int ndCycle(char* sendBuff, int* listenfd) {
    char* device = (char*)malloc(strlen("wlp2s0"));
    pthread_t threadId;
    pthread_create(&threadId, NULL, sniff, (void*)device);
    pthread_join(threadId, NULL);
    while(1) {
        int connfd = accept(*listenfd, (struct sockaddr*)NULL, NULL);
        recv(connfd, sendBuff, 1024, 0);
        write(connfd, sendBuff, strlen(sendBuff));
        close(connfd);
    }

    return 0;
}


int initNdSrv() {
    int listenfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    
    ndCycle(sendBuff, &listenfd);

    return 0;
}
