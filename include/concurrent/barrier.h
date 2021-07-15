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
 * 
 * When wait time not short, use pthread barrier for reduce CPU usage
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
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}

/**
 * @brief  Wait barrier
 * @param  barrier: barrier
 */
__CONCURRENT_INLINE void barrier_wait(barrier_t *barrier)
{
    if (__atomic_sub_fetch(barrier, 1, __ATOMIC_ACQ_REL) > 0) {
        while (1) {
            if (__atomic_load_n(barrier, __ATOMIC_RELAXED) > 0)
                sched_yield();
            else
                break;
        }
    }
}

#endif // __CONCURRENT_BARRIER_H__
