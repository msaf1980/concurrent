#ifndef __CONCURRENT_SPINLOCK_H__
#define __CONCURRENT_SPINLOCK_H__

#include <stdbool.h>
#include <sched.h>
#include <unistd.h>

/**
 * @brief  Spinlock (short-duration userspace lock with atomic)
 */
typedef char spinlock_t;

/**
 * @brief  init spinlock
 * @param  *spinlock: 
 * @retval 
 */
static inline void spinlock_init(spinlock_t *spinlock)
{
    *spinlock = 0;
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}

/**
 * @brief  try to lock spinlock
 * @param  *spinlock: 
 * @retval 0 on success, -1 on failure
 */
static inline int spinlock_lock_try(spinlock_t *spinlock)
{
    //if (__sync_bool_compare_and_swap(spinlock, 0, 1))
    spinlock_t expected = 0;
    // (type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)
    if (__atomic_compare_exchange_n(spinlock, &expected, 1, false, __ATOMIC_ACQ_REL, __ATOMIC_CONSUME))
        return 0;
    else
        return -1;
}

/**
 * @brief  lock spinlock (loop until success)
 * 
 * When failed, yield with sched_yield() and retry. Without yield sometimes can be slow.
 * @param  *spinlock: 
 */
static inline void spinlock_lock(spinlock_t *spinlock)
{
 //   while (! __sync_bool_compare_and_swap(spinlock, 0, 1)) {
    spinlock_t expected = 0;
    while (!__atomic_compare_exchange_n(spinlock, &expected, 1, false, __ATOMIC_ACQ_REL, __ATOMIC_CONSUME))
        sched_yield();
}

/**
 * @brief  lock spinlock (wait up to n * usec microseconds)
 * 
 * When failed, call usleep(usec) (up to n count)
 * @param  *spinlock: 
 * @param  usec: microseconds to sleep
 * @param  n:    count for sleep on failure
 * @retval 0 on success, -1 on failure
 */
static inline int spinlock_lock_wait(spinlock_t *spinlock, useconds_t usec, size_t n)
{
    //while (! __sync_bool_compare_and_swap(spinlock, 0, 1)) {
    spinlock_t expected = 0;
    while (!__atomic_compare_exchange_n(spinlock, &expected, 1, false, __ATOMIC_ACQ_REL, __ATOMIC_CONSUME)) {
        if (n == 0) {
            return -1;
        }
        usleep(usec);
        n--;
    }
    return 0;
}

/**
 * @brief  Unlock spinlock
 * @param  *spinlock: 
 * @retval 0 on success, -1 on error (not unlocked)
 */
static inline int spinlock_unlock(spinlock_t *spinlock)
{
    //if (__sync_val_compare_and_swap(spinlock, 1, 0) == 1)
    spinlock_t expected = 1;
    if (__atomic_compare_exchange_n(spinlock, &expected, 0, false, __ATOMIC_ACQ_REL, __ATOMIC_CONSUME))
        return 0;
    else
        return -1;
}

#endif // __CONCURRENT_SPINLOCK_H__
