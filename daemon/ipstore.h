#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char* ip;
    int count;
    struct node *next;
} node_t;

int getIPCount(char*);
node_t* storeIP(char*, node_t*, FILE*);
void storeIPData(FILE*, node_t*);
node_t* initIPStore(FILE*);
