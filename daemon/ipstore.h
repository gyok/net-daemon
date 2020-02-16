#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char* ip;
    int count;
    struct node *next;
} node_t;

typedef struct deviceStat {
    char* device;
    node_t *ips;
    struct deviceStat *next;
} dStat;

void mdbg(char*);
int getIPCount(char*, node_t*);
node_t* storeIP(char*, node_t*);
void storeIPData(FILE*, dStat*);
void storeIPDataToBuff(int, node_t*);
node_t* initIPStore(FILE*);
