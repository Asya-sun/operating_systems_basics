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


void list_add(list_t *l, char *str) {
    node_t *new = (node_t *) malloc(sizeof(node_t));
    mutex_init(&new->sync);
    new->next = l->first;
    l->first = new;
    memcpy(new->value, str, MAX_LENGTH);
    // printf("node was added\n");
}

void node_lock(node_t *node) {
    mutex_lock(&node->sync);
}


int node_unlock(node_t *node) {
    return mutex_unlock(&node->sync);
}



