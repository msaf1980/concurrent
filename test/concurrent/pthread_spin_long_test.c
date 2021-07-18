/*
 * For check CPU usage on long operations
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>
#if !HAVE_PTHREAD_BARRIER
#include "pthread_barrier.h"
#endif

#include <concurrent/spinlock.h>
#include <concurrent/arch.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

#define LOOP_COUNT 10000

#define V2I (size_t)(void *)
#define I2V (void *) (size_t)

typedef struct {
    char padding1[CACHE_LINE_SIZE - sizeof(int)];
    size_t cnt;
    pthread_spinlock_t pspinlock;
    size_t loops;
    int verify; /* disable check order (for multithread testing) */
    pthread_barrier_t barrier;
} tst_t;

static void *pspinlock_lock_thread(void *in_arg) {
    tst_t *test_data = (tst_t *) in_arg;
    pthread_spinlock_t *spinlock = &test_data->pspinlock;
    pthread_barrier_wait(&test_data->barrier);
    while (__sync_fetch_and_add(&test_data->cnt, 1) < test_data->loops) {
        pthread_spin_lock(spinlock);
        usleep(1);
        pthread_spin_unlock(spinlock);
    } /* while */

    pthread_exit(NULL);
    return NULL; /* don't need this, but keep the compiler happy */
}

void pspinlock_lock_test(size_t loops, unsigned workers) {
    /* Prepare for threading tests */
    int perr;
    uint64_t start, end;
    unsigned i;

    pthread_attr_t thr_attr;
    pthread_t *t_handles;

    tst_t test_data; /* shared with test threads */

    t_handles = malloc(sizeof(pthread_t) * workers);
    ASSERT_TRUE(t_handles != NULL);

    pthread_spin_init(&test_data.pspinlock, 0);

    test_data.loops = loops;
    test_data.cnt = 0;

    ASSERT_EQUAL_D(
        0,
        pthread_barrier_init(&test_data.barrier, NULL, workers + 1),
        "pthread_barrier_init");

    pthread_attr_init(&thr_attr);
    pthread_attr_setdetachstate(&thr_attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < workers; i++) {
        perr = pthread_create(&t_handles[i], &thr_attr, pspinlock_lock_thread,
                              &test_data);
        ASSERT_EQUAL_D(0, perr, "thread create");
    }

    pthread_attr_destroy(&thr_attr);

    pthread_barrier_wait(&test_data.barrier);
    start = getCurrentTime();

    /* wait for threads to complete. */
    for (i = 0; i < workers; i++) {
        perr = pthread_join(t_handles[i], NULL);
        ASSERT_EQUAL_D(0, perr, "enq_thread join");
    }

    end = getCurrentTime();

    pthread_barrier_destroy(&test_data.barrier);

    ASSERT_EQUAL_U_D(test_data.loops + workers, test_data.cnt, "count mismatch");

    printf("(%f ms, %lu iterations, %llu ns/op) ",
           ((double) end - (double) start) / 1000,
           (unsigned long) test_data.loops,
           (unsigned long long) (end - start) * 1000 / test_data.loops);

    free(t_handles);
}

CTEST(pspinlock, lock) {
    pspinlock_lock_test(LOOP_COUNT, 1);
}

CTEST(pspinlock, lock4) {
    pspinlock_lock_test(LOOP_COUNT, 4);
}

CTEST(pspinlock, lock8) {
    pspinlock_lock_test(LOOP_COUNT, 8);
}

CTEST(pspinlock, lock16) {
    pspinlock_lock_test(LOOP_COUNT, 16);
}

CTEST(pspinlock, lock64) {
    pspinlock_lock_test(LOOP_COUNT, 64);
}

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
} /* main */
