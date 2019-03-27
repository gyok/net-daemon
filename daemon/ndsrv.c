#include "ndsrv.h"

struct ndCycleData {
    char* buff;
    int* fd;
};

void* ndCycle(void* args) {
    char* sendBuff = ((struct ndCycleData*)args)->buff;
    int* listenfd = ((struct ndCycleData*)args)->fd;
    char* firstWord;
    char* ip;
    char* thirdWord;
    FILE* fd;
    while(1) {
        int connfd = accept(*listenfd, (struct sockaddr*)NULL, NULL);
        if (recv(connfd, sendBuff, 1024, 0) >= 0) {
            firstWord = strtok(sendBuff, " ");
            printf("%s", firstWord);
            if (strcmp(firstWord, "show") == 0) {
                ip = strtok(NULL, " ");
                thirdWord = strtok(NULL, " ");
                if (strcmp(thirdWord, "count") == 0) {
                    sprintf(sendBuff, "count: %d", getIPCount(ip));
                }
            } else if (strcmp(firstWord, "stop") == 0) {
                fd = fopen("data.txt", "a");
                storeIPData(fd, NULL);
                fclose(fd);
                sprintf(sendBuff, "stop\n");
            } else if (strcmp(firstWord, "--help") == 0) {
                sprintf(sendBuff, "start (packets are being sniffed from now on from default iface(eth0))\n");
            }
        }
        write(connfd, sendBuff, strlen(sendBuff));
        close(connfd);
        //sleep(30);
    }

    return (void*)0;
}


int initNdThreads(char* sendBuff, int* listenfd) {
    pthread_t threadId, ndCycleThread;
    struct ndCycleData ndcd;
    ndcd.buff = sendBuff;
    ndcd.fd = listenfd;
    pthread_create(&ndCycleThread, NULL, ndCycle, (void*)&ndcd);
    pthread_join(ndCycleThread, NULL);
    ndCycle((void*)&ndcd);

    printf("sniff");
    char* device = (char*)malloc(strlen("wlp2s0"));
    sniff((void*)device);
    pthread_create(&threadId, NULL, sniff, (void*)device);
    pthread_join(threadId, NULL);
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
    
    initNdThreads(sendBuff, &listenfd);

    return 0;
}
