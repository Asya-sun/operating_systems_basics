#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


/*
 * Здесь создается глобальная переменная a
 * Потом создается 2 форка от главного процесса
 * В родительском процессе a = 0
 * 
 * в дочерних процессах на родительскую область памяти, в которой 
 * хранятся глобальные переменные, срабатывает механизм
 * copy-on-write: при попытке что-то записать в эту область памяти 
 * создается копия
 *
 * Таким образом, в родительской памяти a всегда остается равной 0
 */

int a;


int main(void) {
    a = 0;

    int pid;
    printf("(parent) pid: %d\n", getpid());
    
    //child 1
    pid = fork();
    
    if (pid == 0) { //child 1
        for (;;){
            a = 5;

            printf("CHILD_1: a = %d\n", a);
            // usleep(1000L * 1L);
        }
    } else if (pid < 0) { 
        printf("fork() failed\n"); 
        exit(1);     
    }

    printf("(parent 1) child 1 was made: %d\n", pid);

    // child 2
    pid = fork();

    if (pid == 0) { //child 2
        for (;;){
            a = 7;

            printf("CHILD_2: a = %d\n", a);
            // usleep(1000L * 1L);
        }
    } else if (pid < 0) { 
        printf("fork() failed\n");      
    }

    printf("(parent 1) child 2 was made: %d\n", pid);

    for (;;){
        // sleep(1);
        printf("PARENT: a = %d\n", a);
    }
    return 0;
}



