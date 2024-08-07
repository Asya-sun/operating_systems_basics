#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int a = 100;


int main(void) {
    int pid;
    printf("(parent) pid: %d\n", getpid());
    sleep(10);

    int b = 5;
    printf("global: address = %p, value = %d\n", &a, a);
    printf("local: address = %p, value = %d\n", &b, b);
    printf("pid in parent proc: %d\n\n", getpid());
    pid = fork();

    if (pid == 0) { //executing in child process
        printf("(child) pid: %d\n", getpid());
        printf("(child) ppid: %d\n", getppid());
        sleep(10);
        printf("(child) global: address = %p, value = %d\n", &a, a);
        printf("(child) local: address = %p, value = %d\n", &b, b);
        a += 5;
        b +=5;
        printf("(child) after change global: address = %p, value = %d\n", &a, a);
        printf("(child) after change local: address = %p, value = %d\n", &b, b);
        sleep(10);
        exit(5);
    } else if (pid > 0) { //executing in parent process
        printf("(parent) global: address = %p, value = %d\n", &a, a);
        printf("(parent) local: address = %p, value = %d\n", &b, b);
        sleep(50);
        int status;
        waitpid(pid, &status, 0);
        printf("(parent) child process exited with status: %i\n", WEXITSTATUS(status));
        sleep(20);
    } else {
        printf("fork() failed\n");
    }
    return 0;
}
