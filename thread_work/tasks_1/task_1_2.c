#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_SLEEP_TIME 2

void *mythread(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
    sleep(THREAD_SLEEP_TIME);
    printf("exiting mythread\n");
    
    return NULL;
}

void *mythreadinfinity(void *arg) {
	printf("mythread %d: Hello from mythread!\n", gettid());
    sleep(THREAD_SLEEP_TIME);
    printf("exiting mythread\n");
    return NULL;
}

void *mythreadret(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythreadret!\n", getpid(), getppid(), gettid());
	sleep(THREAD_SLEEP_TIME);
    int *res = (int *) malloc(sizeof(int));
    *res = 42;
    printf("exiting mythreadret\n");
    return res;
}

void *mythreadhello(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythreadhello!\n", getpid(), getppid(), gettid());
	sleep(THREAD_SLEEP_TIME);
    char *hello = "hello world";
    char *res = malloc(12);
    memcpy(res, hello, 12);
    printf("exiting mythreadhello\n");
    return res;
}


int main() {
	pthread_t tid;
	int err;
    char *thread_ret = NULL;

/*
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	err = pthread_create(&tid, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
    sleep(1);

    
    err = pthread_join(tid, (void **)&thread_ret);
    if (err) { 
        printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
    } else {
        if (thread_ret) {
            printf("pthread return value: %d\n", *thread_ret);
        }
    }

    err = pthread_create(&tid, NULL, mythreadret, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
    sleep(1);

    thread_ret = NULL;
    err = pthread_join(tid, (void **)&thread_ret);
    if (err) { 
        printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
    } else {
        if (thread_ret) {
            printf("pthread return value: %d\n", *thread_ret);
            free(thread_ret);
        }
    }

    err = pthread_create(&tid, NULL, mythreadhello, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
    sleep(1);

    thread_ret = NULL;
    err = pthread_join(tid, (void **)&thread_ret);
    if (err) { 
        printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
    } else {
        if (thread_ret) {
            printf("pthread return value: %s\n", thread_ret);
            free(thread_ret);
        }
    }

*/
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    while (1) {
        
        err = pthread_create(&tid, NULL, mythread, NULL);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
        sleep(1);

        
        err = pthread_join(tid, NULL);
        if (err) { 
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }

	return 0;
}

