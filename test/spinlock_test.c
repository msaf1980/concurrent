#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include <concurrent/spinlock.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

CTEST(spinlock, test) {
    char spinlock;
    spinlock_init(&spinlock);
    ASSERT_EQUAL(0, spinlock);

    spinlock_lock(&spinlock);
    ASSERT_EQUAL(1, spinlock);
    
    ASSERT_EQUAL_D(-1, spinlock_lock_try(&spinlock), "spinlock_lock_try must fail");

    ASSERT_EQUAL_D(0, spinlock_unlock(&spinlock), "spinlock_unlock must success");
    ASSERT_EQUAL(0, spinlock);

    spinlock_lock_try(&spinlock);
    ASSERT_EQUAL(1, spinlock);

    ASSERT_EQUAL_D(-1, spinlock_lock_wait(&spinlock, 10, 2), "spinlock_lock_wait must fail");

    ASSERT_EQUAL_D(0, spinlock_unlock(&spinlock), "spinlock_unlock must success");
}

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
} /* main */
