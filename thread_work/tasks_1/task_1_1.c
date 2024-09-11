#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_NUMBER 5


int GLOBAL_INIT = 7;


void *mythread(void *arg) {
	int local = 5;
	int static local_stat = 8;
	int const local_con = 11;


	printf("*****************************************\n");
	printf("mythread pid: %d ppid :%d tid: %d pthread_self(): %ld Hello from mythread!\n", 
		getpid(), getppid(), gettid(), pthread_self());
	printf("global: %p local: %p local_static: %p local_const: %p\n", 
		&GLOBAL_INIT, &local, &local_stat, &local_con);


	printf("BEFORE CHANGE global: %d local: %d\n", 
		GLOBAL_INIT, local);
	GLOBAL_INIT++;
	local++;

	printf("AFTER CHANGE global: %d local: %d\n", 
		GLOBAL_INIT, local);
	printf("*****************************************\n");
	return NULL;
}

int main() {
	pthread_t my_thread[THREAD_NUMBER];
	int err;

	printf("main pid: %d ppid :%d tid: %d Hello from main!\n", 
		getpid(), getppid(), gettid());

	for (int i = 0; i < THREAD_NUMBER; ++i) {
		printf("GONNE CREATE PTHREAD NOW\n");
		err = pthread_create(&my_thread[i], NULL, mythread, NULL);
		
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}
		printf("PTHREAD WAS CREATED  NOW id: %ld\n", my_thread[i]);
	}
	
	for (int i = 0; i < THREAD_NUMBER; ++i) {
		err = pthread_join(my_thread[i], NULL);
		if (err) {
			printf("main: pthread_join() failed: %s\n", strerror(err));
		}
	}

	return 0;
}

