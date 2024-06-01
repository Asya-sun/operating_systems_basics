#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    int pid;
    printf("(parent) pid: %d\n", getpid());
    sleep(20);
    pid = fork();

    if (pid == 0) { //executing in child process
        printf("(child) pid: %d\n", getpid());
        printf("(child) ppid: %d\n", getppid());
        sleep(15);
        exit(5);
    } else if (pid > 0) { //executing in parent process
        printf("(parent) child pid: %d\n", getpid());
        sleep(30);
    } else {
        printf("fork() failed\n");
    }
    return 0;
}
