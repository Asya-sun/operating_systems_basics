#ifndef LIST_H
#define LIST_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
// #include <pthread.h>
#include "my_mutex.h"

#define MAX_LENGTH 100

typedef struct _Node {
    char value[100];
    struct _Node* next;
    mutex_t sync;
} node_t;

typedef struct _List {
    node_t *first;

    int max_len;
    int cur_len;

} list_t;


list_t* list_init(int max_count); //
void list_destroy(list_t *l); //
void list_add(list_t *l, char *str); //
void node_lock(node_t *node);
int node_unlock(node_t *node);

#endif		// LIST_H