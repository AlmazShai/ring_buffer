#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RB_CODE_BASE   0

#define RB_OK          (RB_CODE_BASE)
#define RB_INVALID_ARG (RB_CODE_BASE + 1U)
#define RB_NOT_INIT    (RB_CODE_BASE + 2U)
#define RB_FULL        (RB_CODE_BASE + 3U)
#define RB_EMPTY       (RB_CODE_BASE + 4U)

typedef uint32_t rb_ret_t;

/**
 * @brief   Main ring buffer structure is used for for performing all kind of operations
 *          on buffer.
 * @note    Must be initialized first using @ref rb_init() fuction.
 * @note    This structure should not be changed externally.
 */
typedef struct ring_buffer
{
    void*  buff;
    size_t cap;
    size_t el_size;
    size_t head;
    size_t tail;
} ring_buffer_t;

/**
 * @brief   Ring buffer iterator structure. Can be used to iterate over existing element
 *          and read values.
 * @note    This structure must be initialized first using @ref rb_init_read_it()
 *          function.
 * @note    This structure should not be changed externally.
 */
typedef struct rb_it
{
    ring_buffer_t* rb;
    size_t         idx;
} rb_it_t;

/**
 * @brief   Initializes a ring buffer.
 *
 * @note    The capacity of the ring buffer will always be one less element than can fit
 *          in a given buffer. This is to prevent the head and tail from overlapping.
 *
 * @param rb        - Pointer to the ring buffer structure
 * @param buff      - Pointer to a buffer allocated by user
 * @param buff_size - Size of the given buffer in bytes
 * @param el_size   - Size of the single element in bytes
 *
 * @retval RB_OK            - Operation success
 * @retval RB_INVALID_ARG   - Invalid argument provided
 */
rb_ret_t rb_init(ring_buffer_t* rb, void* buff, size_t buff_size, size_t el_size);

/**
 * @brief   Adds a new element to the buffer.
 *
 * @param rb    - Pointer to the ring buffer structure
 * @param data  - Pointer to the data of the new item to be written
 *
 * @retval RB_OK        - Operation success
 * @retval RB_NOT_INIT  - Ring buffer structure wasn't initialized
 * @retval RB_FULL      - No free space to add a new element
 */
rb_ret_t rb_add(ring_buffer_t* rb, void* data);

/**
 * @brief   Removes oldest element from the ring buffer.
 *
 * @param rb    - Pointer to the ring buffer structure
 *
 * @retval RB_OK        - Operation success
 * @retval RB_NOT_INIT  - Ring buffer structure wasn't initialized
 * @retval RB_EMPTY     - No elements to remove
 */
rb_ret_t rb_remove(ring_buffer_t* rb);

/**
 * @brief   Checks if ring buffer is full.
 *
 * @param rb    - Pointer to the ring buffer structure
 *
 * @return true - Buffer is full
 * @return false -Buffer is not full
 */
bool rb_is_full(ring_buffer_t* rb);

/**
 * @brief   Checks if ring buffer is empty.
 *
 * @param rb    - Pointer to the ring buffer structure
 *
 * @return true - Buffer is empty
 * @return false - Buffer is not empty
 */
bool rb_is_empty(ring_buffer_t* rb);

/**
 * @brief   Initialize an iterator for reading values from oldest to newest.
 *
 * @details The initialized iterator will point to the tail of the buffer, and it will be
 *          incremented with each read operation using this iterator.
 *
 * @param rb    - Pointer to the ring buffer structure
 * @param it    - Pointer to iterator structure
 *
 * @retval RB_OK            - Operation success
 * @retval RB_NOT_INIT      - Ring buffer structure wasn't initialized
 * @retval RB_INVALID_ARG   - Invalid argument provided
 */
rb_ret_t rb_init_read_it(ring_buffer_t* rb, rb_it_t* it);

/**
 * @brief   Read next value using read iterator.
 *
 * @note    Iterator must be initialized using @ref rb_init_read_it() function.
 *
 * @param it        - Pointer to the iterator structure
 * @param data_out  - Pointer by which the data should be written
 *
 * @retval RB_OK            - Operation success
 * @retval RB_NOT_INIT      - Ring buffer structure wasn't initialized
 * @retval RB_EMPTY         - No more values to read
 */
rb_ret_t rb_get_next_val(rb_it_t* it, void* data_out);

#ifdef __cplusplus
}
#endif

#endif