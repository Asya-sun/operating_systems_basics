#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>


int main(void) {
    int fda[2];

    int err = pipe(fda);
    if (err == -1) {
        printf("pipe failed\n");
        return 1;
    }

    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        return 1;
    } else if (pid == 0) {

        close(fda[0]);
        while (1) {
            unsigned int num = 0;
            for (int i = 0; i < PIPE_BUF / sizeof (char); i++) {
                write(fda[1], &(num), sizeof(unsigned int));
                printf("write: num = %u\n", num);
                fflush(stdout);
                num++;
                sleep(1);
            }
        }
        close(fda[1]);

    } else if (pid > 0) {
        close(fda[1]);
        while (1) {
            unsigned int num = 0;
            for (int i = 0; i < PIPE_BUF / sizeof (char); i++) {
                read(fda[0], &(num), sizeof(unsigned int));
                if (num != i && i !=0 ) {
                    printf("\nlook: %u != (%u+ 1)\n", num, i - 1);
                }
                printf("read: num = %u\n", num);
                fflush(stdout);
                num++;
                sleep(1);
            }
        }
        close(fda[0]);
    }
    return 0;
}
