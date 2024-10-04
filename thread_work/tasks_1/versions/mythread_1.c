#define _GNU_SOURCE
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <malloc.h>
#include <fcntl.h>
#include <ucontext.h>
#include "mythread.h"

__thread mythread_t gtid;

int mythread_cancel(mythread_t tid) {
    mythread_struct_t *mythread = tid;
    printf("cancel: set cancel state to thread %d\n", mythread->mythread_id);
    mythread->canceled = 1;
    mythread->retval = "I'm fine!";
}


void mythread_testcancel(void) {
    mythread_struct_t *mythread;

    mythread = gtid;
    
    if (mythread->canceled) {
        setcontext(&(mythread->before_start_routine));
    }

}


int clear_resources (mythread_struct_t *mythread) {
    printf("i clear resources: stack: %p\n", mythread->stack);
    int res =  munmap(mythread->stack, STACK_SIZE);
    if (res) {
        perror("clear resources\n");
        exit(EXIT_FAILURE);
    }
    return res;
}


int mythread_startup(void *arg) {
    mythread_struct_t *mythread = (mythread_struct_t *) arg;
    gtid = mythread;
    mythread_t tid = mythread;

    printf("startup: starting a thread function for the thread : %d\n", mythread->mythread_id);

    getcontext(&(mythread->before_start_routine));


    if (! mythread->canceled) {
        mythread->retval = mythread->start_routine(mythread->arg);
    }


    mythread->exited = 1;

    printf("startup: thread %d exited type: %d\n", mythread->mythread_id, mythread->type);

    if (mythread->type == DETACHED) {
        mythread->retval = NULL;
        printf("startup: detach thread is %d\n", mythread->mythread_id);
        /**
         * unfortunately, I can't clear stack of a process from the process, sooo I can't clear resources from here
         * // return clear_resources(mythread);
         */
        printf("your detached thread %d is done\n", mythread->mythread_id);
        return 0;
    }
    //if you here it means mythread->type = JOINABLE

    // while (! mythread->joined) {
    //     printf("startup: waiting to be joined %d\n", mythread->mythread_id);
    //     sleep(1);
    // }
    // printf("startup: thread %d was joined bye-bye!\n", mythread->mythread_id);
    return 0;
}


/**
 * The mythread_join() function waits for the thread specified by thread to
 * terminate.  If that thread has already terminated, then  pthread_join()
 * returns immediately.  The thread specified by thread must be joinable.
 */
int mythread_join(mythread_t tid, void **retval) {
    mythread_struct_t *mythread = tid;
    printf("join: thread %d is waited (checked) to be finished\n", mythread->mythread_id);
    while (! mythread->exited) {
        sleep(1);
    }
    printf("join: thread %d was finished\n", mythread->mythread_id);
    if (mythread->retval == NULL || retval == NULL) {
        printf("NULL!!! line: %d\n", __LINE__);
    } else {
        memcpy(*retval, mythread->retval, strlen(mythread->retval) + 1);
    }

    mythread->joined = 1;
    // clear_resources(mythread);
    return 0;
}


int mythread_create(mythread_t *mytid, void *(*start_routine) (void*), void *arg) {
    printf("line: %d\n", __LINE__);
    static int thread_num = 0;
    mythread_struct_t *mythread;
    int child_pid;
    void *child_stack;
    
    thread_num++;

    //creating stack
    child_stack = mmap(NULL, STACK_SIZE, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("child_stack: %p\n", child_stack);

    mprotect(child_stack + PAGE, STACK_SIZE - PAGE, PROT_READ | PROT_WRITE);
    memset(child_stack + PAGE, 0x7f, STACK_SIZE - PAGE); // that's not obvious
    mythread = (mythread_struct_t *)(child_stack + STACK_SIZE - sizeof(mythread_struct_t));
    printf("mythread: %p\n", mythread);
    printf("mythread + sizeof(mythread_struct_t) - STACK_SIZE: %p\n", mythread + sizeof(mythread_struct_t) - STACK_SIZE);
    //got place for structure under stack
    
    mythread->mythread_id = thread_num;
    mythread->start_routine = start_routine;
    mythread->arg = arg;
    mythread->retval = NULL;
    mythread->exited = 0;
    mythread->joined = 0;
    mythread->canceled = 0;
    mythread->type = JOINABLE;
    mythread->stack = child_stack;

    child_stack = (void *)mythread; //mythread in the bottom
                                    //stack will be raising up
    
    child_pid = clone(mythread_startup, child_stack, 
                      CLONE_VM | CLONE_FILES | CLONE_THREAD | CLONE_SIGHAND | SIGCHLD, (void *) mythread);
    
    if (child_pid == -1) {
        printf("clone failed: %s\n", strerror(errno));
        return -1;
    }

    *mytid = mythread;
    return 0;
}

void mythread_detach(mythread_t tid) {
    mythread_struct_t *mythread = tid;
    mythread->type = DETACHED;
    printf("thread %d got DETACHED state\n", tid->mythread_id);
}
