#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

int GLOBAL_INIT = 7;
int GLOBAL_UNINIT;
int const GLOBAL_CONST = 10;


void f(){
    printf("global inited %p\n", &GLOBAL_INIT);
    printf("global uninited %p\n", &GLOBAL_UNINIT);
    printf("global const %p\n", &GLOBAL_CONST);

    int local_init = 5;
    int local_uninit;
    int static local_static;
    int const local_const;

    printf("local inited %p\n", &local_init);
    printf("local uninited %p\n", &local_uninit);
    printf("local static %p\n", &local_static);
    printf("local const %p\n", &local_const);
}

int *create_init_local_var() {
    int a = 7;
    return &a;
}

//e
void heap_function() {
    char *buf_1 = (char *) malloc(100);
    char *phrase = "hello_world";
    memcpy(buf_1, phrase, 11);
    printf("what lies in buf_1 before free: %s\n", buf_1);
    free(buf_1);
    printf("what lies in buf_1 after free: %s\n", buf_1);


    char *buf_2 = (char *) malloc(100);
    memcpy(buf_2, phrase, 11);
    printf("what lies in buf_2 before move and free: %s\n", buf_2);
    buf_2 = buf_2 + 50;
    free(buf_2);
    printf("what lies in buf_2 after move and free: %s\n", buf_2);
}

void environment_var_work() {
    char *ev = getenv("AAAAA");
    printf("ev before change: %s\n", ev);
    setenv("AAAAA", "BBBBB", 1);
    ev = getenv("AAAAA");
    printf("ev after change: %s\n", ev);
}


int main(int argc, char **argv) {
    heap_function();
    printf("pid = %d\n", getpid());
    sleep(600);
    return 0;
}
