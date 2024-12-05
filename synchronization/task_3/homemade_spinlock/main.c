#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdatomic.h>
#include <assert.h>
#include <string.h>

#include <pthread.h>
#include <sched.h>

#include "list.h"

#define LIST_SIZE 100
#define SWAP_THREADS_NUMBER 3

int GLOBAL_ASC = 0;
int GLOBAL_DES = 0;
int GLOBAL_EQUAL = 0;
int GLOBAL_SWAP = 0;
pthread_spinlock_t swap_mutex;

void *ascending(void *arg) {
	list_t *l = (list_t *) arg;

	while (1) {
		node_t *cur = l->first;
		int counter = 0;
		node_lock(cur);


		node_t *next = cur->next;


		while (next != NULL) {
			node_lock(next);
			if (strlen(cur->value) < strlen(next->value)) {
				counter++;
			}
			node_unlock(cur);
			cur = next;
			next = cur->next;
		}

		node_unlock(cur);

		GLOBAL_ASC++;
	}
}

void *descending(void *arg) {
	list_t *l = (list_t *) arg;

	while (1) {
		node_t *cur = l->first;
		int counter = 0;
		node_lock(cur);
		node_t *next = cur->next;


		while (next != NULL) {
			node_lock(next);
			if (strlen(cur->value) > strlen(next->value)) {
				counter++;
			}
			node_unlock(cur);
			cur = next;
			next = cur->next;
		}

		node_unlock(cur);

		GLOBAL_DES++;
	}
    
}

void *equal(void *arg) {
	list_t *l = (list_t *) arg;

	while (1) {
		node_t *cur = l->first;
		node_lock(cur);
		int counter = 0;
		node_t *next = cur->next;


		while (next != NULL) {
			node_lock(next);
			if (strlen(cur->value) == strlen(next->value)) {
				counter++;
			}
			node_unlock(cur);
			cur = next;
			next = cur->next;
		}

		node_unlock(cur);		

		GLOBAL_EQUAL++;
	}
    
}


void *random_swap(void *arg) {
  list_t *storage = (list_t *)arg;
  while (1) {
    node_t *base = storage->first;
    if (!base) {
      fprintf(stderr, "list is empty\n");
      return NULL;
    }
    node_lock(base);
    node_t *first = base->next;
    if (!first) {
      fprintf(stderr, "list contains only 1 element\n");
      return NULL;
    }
    node_lock(first);
    node_t *second = first->next;
    while (second) {
      int should_not_change = rand() % 2;
      if (should_not_change) {
        node_unlock(base);
        base = first;
        first = second;
        node_lock(first);
        second = first->next;
      } else {
        node_lock(second);
        first->next = second->next;
        second->next = first;
        base->next = second;
        node_unlock(base);
        base = second;
        second = first->next;
		pthread_spin_lock(&swap_mutex);
        GLOBAL_SWAP++;
		pthread_spin_unlock(&swap_mutex);
      }
    }
    node_unlock(base);
    node_unlock(first);
  }
}

void *stats_printer(void *arg) {
  while (1) {
    sleep(1);
    printf("GLOBAL_ASC	GLOBAL_DES	GLOBAL_EQUAL	GLOBAL_SWAP\n%d\t%d\t%d\t%d\n", 
			GLOBAL_ASC,	GLOBAL_DES,	GLOBAL_EQUAL,GLOBAL_SWAP);
  }
}

void random_str(char buffer[MAX_LENGTH]) {
	memset(buffer, 0, MAX_LENGTH);
	int rand_len = rand() % MAX_LENGTH;
	for (int i = 0; i < rand_len; ++i) {
		//'!' - 'z' ASCII
		buffer[i] = rand() % 80 + '!';
	}
}

int main() {
	srand(time(NULL));
	pthread_t tid1, tid2, tid3, stat;
	list_t *l;
	int err;
	pthread_t swapping_threads[SWAP_THREADS_NUMBER];
	char buffer[MAX_LENGTH];

	printf("LIST_SIZE: %d\n", LIST_SIZE);
	printf("main [%d %d %d]\n", getpid(), getppid(), gettid());
	l = list_init(LIST_SIZE);

	for (int i = 0; i < LIST_SIZE; ++i) {
		random_str(buffer);
		list_add(l, buffer);
	}

	err = pthread_create(&tid1, NULL, ascending, l);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid2, NULL, descending, l);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid3, NULL, equal, l);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	pthread_spin_init(&swap_mutex, PTHREAD_PROCESS_PRIVATE);
	for (int i = 0; i < SWAP_THREADS_NUMBER; ++i) {
		err = pthread_create(&swapping_threads[i], NULL, random_swap, l);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}
	}

	err = pthread_create(&stat, NULL, stats_printer, l);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	pthread_exit(NULL);
	return 0;
}