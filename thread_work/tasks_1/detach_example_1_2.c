#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_SLEEP_TIME 2

void *mythreaddetouched(void *arg) {
	printf("mythread %d: Hello from mythreaddetouched!\n", gettid());
    sleep(THREAD_SLEEP_TIME);
    printf("exiting mythreaddetouched\n");
    pthread_detach(pthread_self());
    return NULL;
}

void *mythread(void *arg) {
	printf("mythread %d: Hello from mythread!\n", gettid());
    sleep(THREAD_SLEEP_TIME);
    printf("exiting mythread\n");
    return NULL;
}


int main() {
	pthread_t tid;
	int err;
    char *thread_ret = NULL;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
   
    err = pthread_create(&tid, NULL, mythreaddetouched, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }

    sleep(THREAD_SLEEP_TIME + 2);
    printf("continue main\n");


    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &attr, mythread, NULL);
    pthread_attr_destroy(&attr); //destroying attributes after creation of the threadqq
    sleep(THREAD_SLEEP_TIME + 2);
    printf("exiting main\n");
	return 0;
}

