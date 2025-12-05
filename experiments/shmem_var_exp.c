#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

/*
 * Здесь выделяется кусок разделяемой памяти
 * и в него пишется переменная a
 * При форке дочерних процессов в дочерних процессах
 * на области с разделяемой памятью (но это см флаги)
 * механизм copy-on-write не действует, а потому когда
 * в дочерних процессах значение a меняют, то это видно и в родительском,
 * и в дочерних процессах
 *
 */


int main(void) {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *a = shmat(shmid, NULL, 0);
    *a = 0;

    int pid;
    printf("(parent) pid: %d\n", getpid());
    // sleep(10);
    pid = fork();
    //parent 1 ---- child 1

    if (pid == 0) { //child 1
        for (;;){
            *a = 5;

            printf("CHILD_1: a = %d\n", *a);
            // usleep(1000L * 1L);
        }
    } else if (pid < 0) { 
        printf("fork() failed\n");      
    }

    printf("(parent 1) child 1 was made: %d\n", pid);

    // child 2
    pid = fork();

    if (pid == 0) { //child 1
        for (;;){
            *a = 7;

            printf("CHILD_2: a = %d\n", *a);
            // usleep(1000L * 1L);
        }
    } else if (pid < 0) { 
        printf("fork() failed\n");      
    }

    printf("(parent 1) child 2 was made: %d\n", pid);

    for (;;){
        // sleep(1);
        printf("PARENT: a = %d\n", *a);
    }
    return 0;
}



