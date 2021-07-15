#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include <concurrent/barrier.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

static void *barrier_wait_thread(void *in_arg) {
    barrier_wait((barrier_t *) in_arg);
}

#define WORKERS 10

CTEST(barrier, test) {
    int perr;
    barrier_t barrier;

    uint64_t start, end;
    size_t i;
    pthread_attr_t thr_attr;
    pthread_t t_handles[WORKERS];

    barrier_init(&barrier, WORKERS+1);
    ASSERT_EQUAL(WORKERS+1, barrier);

    pthread_attr_init(&thr_attr);
    pthread_attr_setdetachstate(&thr_attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < WORKERS; i++) {
        perr = pthread_create(&t_handles[i], &thr_attr, barrier_wait_thread, &barrier);
        ASSERT_EQUAL_D(0, perr, "thread create");
    }

    start = getCurrentTime();

    barrier_wait(&barrier);

    end = getCurrentTime();

    for (i = 0; i < WORKERS; i++) {
        pthread_join(t_handles[i], NULL);
    }

    ASSERT_EQUAL(0, barrier);

    printf("(%f ms, %d count) ",
           ((double) end - (double) start) / 1000,
           (unsigned long) WORKERS+1);
}

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
} /* main */
