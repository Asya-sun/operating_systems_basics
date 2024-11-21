#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_SLEEP_TIME 5

typedef struct {
    int a;
    char *str;
} thread_args;

void *mythreaddetach(void *_args) {
    pthread_detach(pthread_self());
    thread_args *args = (thread_args *) _args;
	printf("mythreaddetach [%d %d %d]: Hello from mythreaddetach!\n", getpid(), getppid(), gettid());
	printf("mythreaddetach: args->a: %d\nargs->str: %s\n", args->a, args->str);
    sleep(THREAD_SLEEP_TIME);
    free(args);
    printf("exiting mythreaddetach()\n");
    return NULL;
}

void *mythread(void *_args) {
    thread_args *args = (thread_args *) _args;
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	printf("mythread: args->a: %d\nargs->str: %s\n", args->a, args->str);
    sleep(THREAD_SLEEP_TIME + 5);
    printf("exiting mythread()\n");
    return NULL;
}

int main() {
	pthread_t tid;
	int err;

    thread_args *args = malloc(sizeof(thread_args));
    args->a = 5;
    args->str = "hello world";


	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	err = pthread_create(&tid, NULL, mythreaddetach, args);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}


    pthread_t my_tid;
    thread_args my_args;
    my_args.a =100;
    my_args.str = "hellooooo!!!";


    err = pthread_create(&my_tid, NULL, mythread, &my_args);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

    err = pthread_join(my_tid, NULL);
    if (err) {
	    printf("main: pthread_join() failed: %s\n", strerror(err));
		return -1;
    }
    printf("exit main\n");
    pthread_exit(NULL);

	return 0;
}

