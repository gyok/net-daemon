#include <stdio.h>
#include <time.h>
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <unistd.h>
#include <pthread.h>
#include "ipstore.h"

#define SIZE_ETHERNET 14

struct ndData {
    char* buff;
    char* device;
    int* fd;
    pthread_mutex_t mutex;
    node_t* ips;
};

void* sniff(void*);
