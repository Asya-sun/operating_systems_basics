#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    int pid;
    printf("(parent) pid: %d\n", getpid());
    sleep(10);
    pid = fork();
    //parent 1 ---- child 1

    if (pid == 0) { //child 1
        sleep(5);
        int next_pid = fork();
        //child 1=parent 2 ---- child 2
        
        
        if (next_pid == 0) { //child 2
            printf("(child 2) pid: %d\n", getpid());
            printf("(child 2) ppid: %d\n", getppid());
            sleep(60);
            exit(0);
        } else if (next_pid > 0) { //parent 2=child 1
            printf("(child 1=parent 2) child 2 was made: %d\n", next_pid);
            printf("(child 1=parent 2) pid: %d\n", getpid());
            //printf("(child 1=parent 2) ppid: %d\n", getppid());
            sleep(10);
            printf("child 1=parent 2 was destroyed\n");
            exit(0);
        } else {
            printf("fork() 2 failed\n");
            exit(0);
        }
    } else if (pid > 0) { //parent 1
        printf("(parent 1) child 1 was made: %d\n", pid);
        sleep(70);
        int status;
        /*
        waitpid(pid, &status, 0);
        if (status == 1) {
            printf("fork() 2 failed\ntry again\n");
            return 0;
        } else if (status == 0) {
            sleep(70);
            printf("(parent) child pid: %d\n", getpid());
            sleep(30);
        }
         */
    } else {
        printf("fork() failed\n");
    }
    return 0;
}
