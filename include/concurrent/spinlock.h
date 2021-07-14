#ifndef __CONCURRENT_SPINLOCK_H__
#define __CONCURRENT_SPINLOCK_H__

#include <sched.h>
#include <unistd.h>

/**
 * @brief  init spinlock
 * @param  *spinlock: 
 * @retval 
 */
static inline void spinlock_init(char *spinlock)
{
    *spinlock = 0;
    __sync_synchronize();
}

/**
 * @brief  try to lock spinlock
 * @param  *spinlock: 
 * @retval 0 on success, -1 on failure
 */
static inline int spinlock_lock_try(char *spinlock)
{
    if (__sync_bool_compare_and_swap(spinlock, 0, 1))
        return 0;
    else
        return -1;
}

/**
 * @brief  lock spinlock (loop until success)
 * 
 * when failed, call sched_yield() and retry
 * @param  *spinlock: 
 */
void spinlock_lock(char *spinlock)
{
    while (! __sync_bool_compare_and_swap(spinlock, 0, 1)) {
        sched_yield();
    }
}

/**
 * @brief  lock spinlock (wait up to n * usec microseconds)
 * 
 * When failed, call usleep(usec) (up to n count)
 * @param  *spinlock: 
 * @param  usec microseconds to sleep
 * @param  n    count for sleep on failure
 * @retval 0 on success, -1 on failure
 */
static inline int spinlock_lock_wait(char *spinlock, useconds_t usec, size_t n)
{
    while (! __sync_bool_compare_and_swap(spinlock, 0, 1)) {
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
static inline int spinlock_unlock(char *spinlock)
{
    if (__sync_val_compare_and_swap(spinlock, 1, 0) == 1)
        return 0;
    else
        return -1;
}

#endif // __CONCURRENT_SPINLOCK_H__
