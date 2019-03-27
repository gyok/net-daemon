#include "ipstore.h"

node_t* initIPStore(FILE* f) {
    node_t *head, *current;
    head = current = NULL;
    char* ip = malloc(sizeof(char) * 50);;
    int count = 0;

    while (fscanf(f, "%50s %d", ip, &count) != EOF) {
        node_t* newNode = malloc(sizeof(node_t));
        newNode->ip = malloc(sizeof(char) * 50);
        strcpy(newNode->ip, ip);
        newNode->count = count;
        newNode->next = NULL;

        if (head == NULL) {
            current = head = newNode;
        } else {
            current = current->next = newNode;
        }
    }
    return head;
}


void storeIPData(FILE* f, node_t* ips) {
    node_t *current = ips;
    if (ips != NULL) {
        for (current = ips; current; current = current->next) {
            fprintf(f, "%50s %d\n", current->ip, current->count);
        }
    }
}

int getIPCount(char* ip) {
    node_t *current = NULL;
    while (current != NULL) {
        if (strcmp(current->ip, ip) == 0) {
            return current->count;
        }
        current = current->next;
    }

    return 0;
}

node_t* storeIP(char* ip, node_t* ips, FILE* f) {
    node_t* current = ips;

    node_t* newNode = malloc(sizeof(node_t));
    newNode->ip = malloc(sizeof(char) * 50);
    // empty list
    if (current == NULL) {
        newNode->count = 1;
        strcpy(newNode->ip, ip);
        newNode->next = NULL;
        ips = newNode;
        return ips;
    }

    int compareRes;
    while (current->next != NULL) {
        compareRes = strcmp(current->ip, ip);
        if (compareRes == 0) {
            current->count++;
            return ips;
        }
        if (compareRes > 0) {
            newNode->next = current->next;
            strcpy(newNode->ip, current->ip);
            newNode->count = current->count;
            current->count = 1;
            strcpy(current->ip, ip);
            current->next = newNode;
            return ips;
        }
        current = current->next;
    }
    newNode->count = 1;
    strcpy(newNode->ip, ip);
    current->next = newNode;
    return ips;
}
