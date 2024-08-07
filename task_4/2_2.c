#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>

int ARRAY_SIZE = 100;
int BUF_SIZE = 409600;
int PAGE_SIZE = 4096;

void stack_f() {
    char a[4096];
    sleep(1);
    stack_f();
}

void heap_f() {
    char *array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = malloc(BUF_SIZE);
        usleep(100000);
    }
    for (int i = ARRAY_SIZE - 1; i >= 0; i--) {
        free(array[i]);
    }

}

void region_f() {
    int *a = mmap(NULL, 10 * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    printf("10 pages were mapped\n");
    sleep(10);
    mprotect(a + PAGE_SIZE, PAGE_SIZE, PROT_NONE);
    printf("page 1 was mprotected (PROT_NONE)\n");
    //printf("trying to read\n");
    //int b = *(a + PAGE_SIZE);
    //printf("trying to write\n");
    //*(a + PAGE_SIZE + 4) = 5;
    munmap(a + 3 * PAGE_SIZE, 3 * PAGE_SIZE);
    printf("pages 4-6 were unmapped\n");
}


void handle_sigsegv(int a) {
    perror("sigsegv");
    exit(1);
}

int main(void) {
    printf("pid: %d\n", getpid());
    sleep(20);
    signal(SIGSEGV, handle_sigsegv);
    printf("region_f\n");
    region_f();
    sleep(10);
    printf("heap_f\n");
    heap_f();
    sleep(10);
    printf("stack_f\n");
    stack_f();
    return 0;
}
