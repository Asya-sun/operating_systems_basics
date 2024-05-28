#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define FILE_SIZE 4096
#define PAGE_SIZE 4096
#define type unsigned int

int main(void) {

    int fd = open("./my_file", O_RDWR | O_CREAT, 0666);
    //сбросили размер файла до 0
    ftruncate(fd, 0);
    //размер файла = FILE_SIZE
    ftruncate(fd, FILE_SIZE);

    void *memory = mmap(NULL, FILE_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    type *buffer = (type *)memory;

    int pid = fork();

    if (pid < 0) {
        printf("error fork()\n");
        return 1;
    } else if (pid == 0) {
        while (1) {
            for (int i = 0; i < FILE_SIZE / sizeof(type) ;i++) {
                buffer[i] = (type) i;
                sleep(1);
            }
        }
    } else if (pid > 0) {
        while (1) {
            type expected = 0;
            for (int i = 0; i < FILE_SIZE/ sizeof(type) ;i++) {
                type now = buffer[i];
                if ((now  != (expected + 1)) && (i != 0)) {
                    printf("\nlook: %u != (%u+ 1)\n", now, expected);
                }
                printf("%u ", now);
                fflush(stdout);
                sleep(1);
                expected = (type) i;
            }
        }
    }

    munmap(memory, PAGE_SIZE);
    return 0;
}
