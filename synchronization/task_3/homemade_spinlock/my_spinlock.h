#ifndef MY_SPINLOCK_H
#define MY_SPINLOCK_H

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


#endif		// MY_SPINLOCK_H