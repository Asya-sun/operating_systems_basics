#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define NUM_THREADS 2

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); pthread_exit(NULL); } while (0)

// Обработчик сигнала SIGINT
void sigint_handler(int signum) {
    printf("SIGINT was got by the thread %ld\n", pthread_self());
    pthread_exit(NULL);
}

// handling SIGINT
void *thread_handle_sigint(void *arg) {    
    sigset_t set;

    sigfillset(&set);
    sigdelset(&set, SIGINT);

    pthread_sigmask(SIG_SETMASK, &set, NULL);

    signal(SIGINT, sigint_handler);
    printf("pthread waiting for SIGINT %ld\n", pthread_self());
    while (1) {
        sleep(1); // Бесконечный цикл
    }
    pthread_exit(NULL);
}

// waiting for SIGQUIT
void *thread_wait_sigquit(void *arg) {
    sigset_t set;
    int s;
    int sig;
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    pthread_sigmask(SIG_UNBLOCK, &set, NULL);
    printf("pthread waiting for SIGQUIT %ld\n", pthread_self());


    s = sigwait(&set, &sig);
    if (s != 0) {
        handle_error_en(s, "sigwait");
    }
    printf("SIGQUIT was got in pthread %ld\n", pthread_self());

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    sigset_t set;
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    
    pthread_create(&threads[0], NULL, thread_handle_sigint, NULL);
    pthread_create(&threads[1], NULL, thread_wait_sigquit, NULL);

    int pid = getpid();
    sleep(2);
    kill(pid, SIGINT);
    sleep(2);
    kill(pid, SIGQUIT);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}