#ifndef __CONCURRENT_BARRIER_H__
#define __CONCURRENT_BARRIER_H__

#include <sched.h>
#include <unistd.h>

typedef int barrier_t;

/**
 * @brief  init barrier
 * @param  *barrier:
 * @param  n: wait 
 * @retval 
 */
static inline void barrier_init(barrier_t *barrier, int count)
{
    *barrier = count;
    __sync_synchronize();
}

/**
 * @brief  wait barrier
 * @param  *barrier: 
 */
static inline void barrier_wait(barrier_t *barrier)
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
