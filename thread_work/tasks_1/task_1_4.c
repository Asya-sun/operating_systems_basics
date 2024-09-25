#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


void *mythreadstring(void *arg) {
	printf("mythreadstring [%d %d %d]: Hello from mythreadstring!\n", getpid(), getppid(), gettid());
    while(1) {
        printf("mythreadstring: printing string\n");
        // sleep(1);
    }
	return NULL;
}

void cleanup_routine(void *arg) {
	free((char *) arg);
}

void *mythreadinfinity(void *arg) {
	//printf("mythreadinfinity [%d %d %d]: Hello from mythreadinfinity!\n", getpid(), getppid(), gettid());
    int err = pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if (err) { 
		perror("pthread_setcanceltype");
		exit(1);
	}
	int a = 0;
	while(1) {
        a++;
    }
	return NULL;
}


void *mythreadhello(void *arg) {
	printf("mythreadhello [%d %d %d]: Hello from mythreadhello!\n", getpid(), getppid(), gettid());
    char *str = malloc(sizeof("hello world"));
	memcpy(str, "hello world", sizeof("hello world"));

	pthread_cleanup_push(cleanup_routine, (void*)str);
	
	while(1) {
        printf("%s\n", str);
    }
	//this string will never be reached up bc pthread_cancel()
	pthread_cleanup_pop(1);
	return NULL;
}




int main() {
	pthread_t tid;
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	sleep(10); 

	err = pthread_create(&tid, NULL, mythreadstring, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
    sleep(3);
    err = pthread_cancel(tid);
    if (err) {
	    printf("main: pthread_cancel() failed: %s\n", strerror(err));
		return -1;
	}


	err = pthread_create(&tid, NULL, mythreadinfinity, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
    sleep(3);
    err = pthread_cancel(tid);
    if (err) {
	    printf("main: pthread_cancel() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid, NULL, mythreadhello, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
    sleep(3);
    err = pthread_cancel(tid);
    if (err) {
	    printf("main: pthread_cancel() failed: %s\n", strerror(err));
		return -1;
	}
	printf("thread mythreadhello was canceled\n");
	return 0;
}

