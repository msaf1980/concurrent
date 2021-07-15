#ifndef MPMC_RING_QUEUE_H
#define MPMC_RING_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <concurrent/queuedef.h>

/**
 * @file
*
* Public header
*/

/**
 * @typedef mpmc_ring_queue
 * @brief  Lock-free, non-blocking message MPMC queue (based on fixed size ring buffer)
 * 
 * This implementation is based on the bounded MPMC queue proposed by Dmitry Vyukov
 * http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
 * 
 * Producers and consumers operate independently, but the operations are not
 * lock-free, i.e., a enqueue may have to wait for a pending dequeue
 * operation to finish and vice versa.
 */
typedef struct mpmc_q_s mpmc_ring_queue;


/**
 * @brief Create an instance of a ring queue.
 * @param q_size: Number of queue elements to allocate.
 * 
 * Must be > 1 and a power of 2.  Due to the nature of the algorithm used, a maximum of
 * q_size - 1 elements can actually be stored in the queue.
 * @param qerr: QERR_OK on success, or other QERR_* value on error. Can be NULL - no error code returned
 * @retval Pointer to caller's queue instance handle or NULL on error.
 */
mpmc_ring_queue *mpmc_ring_queue_new(size_t q_size, qerr_t *qerr);

/**
 * @brief Delete an instance of a queue (items not freed, call mpmc_ring_queue_delete in this case).
 * @param *q: Queue instance handle.
 * @retval QERR_OK on success, or other QERR_* value on error.
 */
qerr_t mpmc_ring_queue_destroy(mpmc_ring_queue *q);

/**
 * @brief Delete an instance of a queue (call mpmc_ring_queue_destroy) and free all items before.
 * @param *q: Queue instance handle.
 * @param f: pointer to free function
 * @retval QERR_OK on success, or other QERR_* value on error.
 */
qerr_t mpmc_ring_queue_delete(mpmc_ring_queue *q, queue_free_func f);

/**
 * @brief Add a message to the queue.
 * @param *q: Queue instance handle.
 * @param *m : Message to enqueue (don't pass NULL).
 * @retval QERR_OK on success, QERR_FULL if queue full, or other QERR_* value on error.
 */
qerr_t mpmc_ring_queue_enqueue(mpmc_ring_queue *q, void *m);

/**
 * @brief Remove a message from the queue.
 * @param q: Queue instance handle.
 * @retval dequeued message or NULL if queue empthy
 */
void *mpmc_ring_queue_dequeue(mpmc_ring_queue *q);

/**
 * @brief Check if queue is empthy.
 * @param q: Queue instance handle.
 * @retval 1 - empty, 0 - not empty
 */
int mpmc_ring_queue_empty(mpmc_ring_queue *q);

/**
 * @brief Check if queue is full.
 * @param q: Queue instance handle.
 * @retval 1 - full, 0 - not full
 */
int mpmc_ring_queue_full(mpmc_ring_queue *q);

/**
 * @brief Return queue capacity.
 * @param q: Queue instance handle.
 * @retval queue size
 */
size_t mpmc_ring_queue_size(mpmc_ring_queue *q);

/**
 * @brief Get queue free length (not full consistent).
 * 
 * Also can return 0 if queue is full or capacity if queue is empthy due to paralell enqueue/dequeue.
 * @param q: Queue instance handle.
 * @retval queue free length
 */
size_t mpmc_ring_queue_free_relaxed(mpmc_ring_queue *q);

/**
 * @brief Get queue length (not full consistent).
 * 
 * Also can return 0 if queue is full or capacity if queue is empthy due to paralell enqueue/dequeue.
 * @param q: Queue instance handle.
 * @retval queue length
 */
size_t mpmc_ring_queue_len_relaxed(mpmc_ring_queue *q);

#ifdef __cplusplus
}
#endif

#endif /* MPMC_RING_QUEUE_H */
