#ifndef MYTHREAD_H
#define MYTHREAD_H

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


#define PAGE 4096
#define STACK_SIZE PAGE * 5 



typedef void *(*start_routine_t)(void *);

enum THREAD_TYPE {
    JOINABLE,
    DETACHED
};

typedef struct mythread_struct_t {
    int mythread_id;
    start_routine_t start_routine;
    void* arg;
    void* retval;
    int joined;  //volatile ?
    int exited;  //volatile ?
    int canceled;
    void *stack;
    ucontext_t uc;

    enum THREAD_TYPE type;
    
    ucontext_t before_start_routine;
} mythread_struct_t;

typedef mythread_struct_t* mythread_t; //указатель на управляющую структуру

mythread_t global_thread_id;

int mythread_startup(void *arg) ;

int mythread_join(mythread_t tid, void **retval);

int mythread_cancel(mythread_t tid);

int mythread_create(mythread_t *mytid, void *(*start_routine) (void*), void *arg);

int clear_resources (mythread_struct_t *mythread);

void mythread_detach(mythread_t tid);

void mythread_testcancel(void);

#endif //MYTHREAD_H