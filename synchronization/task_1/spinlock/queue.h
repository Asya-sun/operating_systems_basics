#ifndef __FITOS_QUEUE_H__
#define __FITOS_QUEUE_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdatomic.h>

typedef struct {
	int lock;
} spinlock_t;

void spinlock_init(spinlock_t *s);

void spinlock_lock(spinlock_t *s);

void spinlock_unlock(spinlock_t *s);

typedef struct _QueueNode {
	int val;
	struct _QueueNode *next;
} qnode_t;

typedef struct _Queue {
	qnode_t *first;
	qnode_t *last;

	spinlock_t s;

	pthread_t qmonitor_tid; //поток печатает значения, чтобы наблюдать

	int count; //текущее число элеиментов в очереди
	int max_count; //max число элеиментов в очереди

	// queue statistics
	long add_attempts;
	long get_attempts;
	long add_count;
	long get_count;
} queue_t;

queue_t* queue_init(int max_count);
void queue_destroy(queue_t *q);
int queue_add(queue_t *q, int val);
int queue_get(queue_t *q, int *val);
void queue_print_stats(queue_t *q);

#endif		// __FITOS_QUEUE_H__
