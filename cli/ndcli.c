#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
    
    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n [E]: could not create socket\n");
        return 1;
    }

    memset (&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\n [E]: inet_pton error occured\n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n [E]: connect failed\n");
        return 1;
    }


    int msgLength = 0;
    for (int i = 1; i < argc; i++) {
        msgLength += strlen(argv[i]);
    }
    // add spaces to length
    msgLength += argc;

    char* msgToSend;
    if (argc > 1) {
        msgToSend = malloc(msgLength);
        strcpy(msgToSend, argv[1]);
        for (int i = 2; i < argc; i++) {
            strcat(msgToSend, " ");
            strcat(msgToSend, argv[i]);
        }
    } else {
        msgToSend = "";
    }

    if (send(sockfd, msgToSend, msgLength, 0) < 0) {
        printf("\n [E]: send failed\n");
    }

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
        recvBuff[n] = 0;
        if (fputs(recvBuff, stdout) == EOF) {
            printf("\n [E]: fputs error\n");
        }
    }

    if (n < 0) {
        printf("\n [E]: read error\n");
    }

    if (argc > 1) {
        free(msgToSend);
    }

    return 0;
}
