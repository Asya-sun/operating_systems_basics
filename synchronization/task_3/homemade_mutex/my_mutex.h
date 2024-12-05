#ifndef MY_MUTEX_H
#define MY_MUTEX_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdatomic.h>

#include <linux/futex.h>
#include <sys/syscall.h>

#define NUMBER_PREFERABLE_TRYES 5


typedef struct {
	// in this realization, lock = owner_tid
	// so if mutex is free, lock = 0
	// if not, lock = tid
	atomic_int lock; // bc atomic functions wanna use atomic types
} mutex_t;


void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
int mutex_unlock(mutex_t *m);


#endif		// MY_MUTEX_H
