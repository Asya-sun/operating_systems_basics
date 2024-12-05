#define _GNU_SOURCE
#include <pthread.h>
#include <assert.h>

#include "list.h"

list_t *list_init(int max_count) {
    list_t *l = malloc(sizeof(list_t));
    
    if (!l) {
        printf("Cannot allocate memory for a list\n");
		abort();
    }

    l->first = NULL;
    l->max_len = max_count;
    l->cur_len = 0;

    printf("list was initialized\n");
    return l;
}

void list_destroy(list_t* l) {
    printf("list starting to be destroyed\n");
    node_t* cur = l->first;
    node_t *next;

    while (cur != NULL) {
        next = cur->next;
        pthread_rwlock_destroy(&cur->sync);
        free(cur);
        cur = next;
    }
    printf("list was destroyed\n");
    free(l);
}

void list_add(list_t *l, char *str) {
    node_t *new = (node_t *) malloc(sizeof(node_t));
    pthread_rwlock_init(&new->sync, NULL);
    new->next = l->first;
    l->first = new;
    memcpy(new->value, str, MAX_LENGTH);
    // printf("node was added\n");
}


void rlock_node(node_t *node) {
    pthread_rwlock_rdlock(&node->sync);
}

void wlock_node(node_t *node) {
    pthread_rwlock_wrlock(&node->sync);
}
void unlock_node(node_t *node) {
    pthread_rwlock_unlock(&node->sync);
}



