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

#define V2I (size_t)(void *)
#define I2V (void *) (size_t)

typedef struct {
    char padding1[CACHE_LINE_SIZE - sizeof(int)];
    size_t cnt;
    char spinlock;
    size_t loops;
    int verify; /* disable check order (for multithread testing) */
    pthread_barrier_t barrier;
} tst_t;

static void *spinlock_lock_unlock_thread(void *in_arg) {
    tst_t *test_data = (tst_t *) in_arg;
    char *spinlock = &test_data->spinlock;
    pthread_barrier_wait(&test_data->barrier);
    while (__sync_fetch_and_add(&test_data->cnt, 1) < test_data->loops) {
        spinlock_lock(spinlock);
        spinlock_unlock(spinlock);
    } /* while */

    pthread_exit(NULL);
    return NULL; /* don't need this, but keep the compiler happy */
} /* spinlock_thread */


void spinlock_lock_unlock(size_t loops, unsigned workers) {
    /* Prepare for threading tests */
    int perr;
    uint64_t start, end;
    unsigned i;

    pthread_attr_t thr_attr;
    pthread_t *t_handles;

    tst_t test_data; /* shared with test threads */

    t_handles = malloc(sizeof(pthread_t) * workers);
    ASSERT_NOT_NULL(t_handles);

    spinlock_init(&test_data.spinlock);

    test_data.loops = loops;
    test_data.cnt = 0;

    ASSERT_EQUAL_D(
        0,
        pthread_barrier_init(&test_data.barrier, NULL, workers + 1),
        "pthread_barrier_init");

    pthread_attr_init(&thr_attr);
    pthread_attr_setdetachstate(&thr_attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < workers; i++) {
        perr = pthread_create(&t_handles[i], &thr_attr, spinlock_lock_unlock_thread,
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

    ASSERT_EQUAL_D(test_data.loops + workers, test_data.cnt, "count mismatch");

    printf("(%f ms, %lu iterations, %llu ns/op) ",
           ((double) end - (double) start) / 1000,
           (unsigned long) test_data.loops,
           (unsigned long long) (end - start) * 1000 / test_data.loops);
}

CTEST(spinlock, lock_unlock) {
    spinlock_lock_unlock(10000000, 1);
}

CTEST(queue_thread, lock_unlock4) {
    spinlock_lock_unlock(10000000, 4);
}

CTEST(queue_thread, lock_unlock8) {
    spinlock_lock_unlock(10000000, 8);
}

CTEST(queue_thread, lock_unlock16) {
    spinlock_lock_unlock(10000000, 16);
}

CTEST(queue_thread, lock_unlock64) {
    spinlock_lock_unlock(10000000, 64);
}

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
} /* main */
