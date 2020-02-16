#include "ndsrv.h"


struct threadData {
    pthread_mutex_t mutex;
    node_t* ips;
}; 

void* ndCycle(void* args) {
    struct ndData *ndcd = (struct ndData*)args;
    char* sendBuff = ndcd->buff;
    int* listenfd = ndcd->fd;
    char* firstWord;
    char* ip;
    char* thirdWord;
    pthread_t sniffThread;
    FILE* fd;
    while(1) {
        int connfd = accept(*listenfd, (struct sockaddr*)NULL, NULL);
        if (recv(connfd, sendBuff, 1024, 0) >= 0) {
            firstWord = strtok(sendBuff, " ");
            if (strcmp(firstWord, "start") == 0) {
                pthread_create(&sniffThread, NULL, sniff, args);
            } else if (strcmp(firstWord, "stop") == 0) {
                fd = fopen("data.txt", "w");
                pthread_mutex_lock(&ndcd->mutex);
                storeIPData(fd, ndcd->ds);
                pthread_mutex_unlock(&ndcd->mutex);
                fprintf(fd, "\nclose\n");
                fclose(fd);

                sprintf(sendBuff, "bye\n");
                write(connfd, sendBuff, strlen(sendBuff));
                close(connfd);
                
                pthread_cancel(sniffThread);
                pthread_join(sniffThread, NULL);

                return (void*)0;
            } else if (strcmp(firstWord, "show") == 0) {
                ip = strtok(NULL, " ");
                thirdWord = strtok(NULL, " ");
                if (strcmp(thirdWord, "count") == 0) {
                    pthread_mutex_lock(&ndcd->mutex);
                    if (ndcd->ds != NULL) {
                        sprintf(sendBuff, "%d\n", getIPCount(ip, ndcd->ds->ips));
                    } else {
                        sprintf(sendBuff, "%d\n", 0);
                    }
                    pthread_mutex_unlock(&ndcd->mutex);
                }
            } else if (strcmp(firstWord, "stat") == 0) {
                    pthread_mutex_lock(&ndcd->mutex);
                    if (ndcd->ds != NULL) {
                        storeIPDataToBuff(connfd, ndcd->ds->ips);
                    }
                    pthread_mutex_unlock(&ndcd->mutex);
            } else if (strcmp(firstWord, "--help") == 0) {
                sprintf(sendBuff, "start (packets are being sniffed from now on from default iface(eth0))\n");
            }
        }
        write(connfd, sendBuff, strlen(sendBuff));
        close(connfd);
        //sleep(30);
    }
    // pthread_exit(0);

    return (void*)0;
}


int initNdThreads(char* sendBuff, int* listenfd) {
    pthread_t ndCycleThread;

    struct ndData ndcd;
    ndcd.device = (char*)malloc(30);
    strcpy(ndcd.device, "wlp2s0");
    ndcd.buff = sendBuff;
    ndcd.fd = listenfd;
    ndcd.ds = NULL;
    pthread_mutex_init(&(ndcd.mutex), NULL);

    pthread_create(&ndCycleThread, NULL, ndCycle, &ndcd);

    pthread_join(ndCycleThread, NULL);


    pthread_mutex_destroy(&(ndcd.mutex));
    
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
