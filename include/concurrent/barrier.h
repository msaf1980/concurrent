#ifndef __CONCURRENT_BARRIER_H__
#define __CONCURRENT_BARRIER_H__

#include <sched.h>
#include <unistd.h>

#include <concurrent/define.h>

/**
 * @file
*
* Public header
*/

/**
 * @typedef barrier_t
 * @headerfile barrier.h <concurrent/barrier.h>
 * @brief  Spinlock based wait barrier
 */
typedef int barrier_t;

/**
 * @brief  Init barrier
 * @param  barrier: barrier
 * @param  count : waiters count
 */
__CONCURRENT_INLINE void barrier_init(barrier_t *barrier, int count)
{
    *barrier = count;
    __sync_synchronize();
}

/**
 * @brief  Wait barrier
 * @param  barrier: barrier
 */
__CONCURRENT_INLINE void barrier_wait(barrier_t *barrier)
{
    if (__sync_sub_and_fetch(barrier, 1) > 0) {
        while (1) {
            if (__sync_add_and_fetch(barrier, 0) > 0)
                sched_yield();
            else
                break;
        }
    }
}

#endif // __CONCURRENT_BARRIER_H__
