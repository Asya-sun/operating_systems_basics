#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <signal.h>
// #include <sched.h>
// #include <errno.h>
// #include <sys/types.h>
// #include <sys/mman.h>
// #include <malloc.h>
// #include <fcntl.h>
// #include <ucontext.h>
#include "mythread.h"


void *mythread(void *arg) {
    printf("getpid: %d %d %d\n", getpid(), getgid(), gettid());
    char *str = (char *) arg;
    
    for (int i = 0; i < 5; i++) {
        mythread_testcancel();
        printf("%s: на станцию прибыл Годжо Сатору\n", str);
        sleep(1);
    }

    return "hahaha";
}

int main() {
    printf("getpid: %d %d %d\n", getpid(), getgid(), gettid());
    mythread_t mytid;
    char retval[1024];
    char *to_retval = &(retval[0]);
    int res;

    int r = mythread_create(&mytid, mythread, "20:31");

    sleep(2);

    // mythread_detach(mytid);
    // sleep(6);

    // mythread_cancel(mytid);
    // usleep(100000);
    res = mythread_join(mytid, (void **)&to_retval);
    printf("main: returned value from mythread1: %s\n", (char *)retval);
}