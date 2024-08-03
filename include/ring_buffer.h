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

typedef struct ring_buffer
{
    void*  buff;
    size_t cap;
    size_t el_size;
    size_t head;
    size_t tail;
} ring_buffer_t;

typedef struct rb_it
{
    ring_buffer_t* rb;
    size_t         idx;
} rb_it_t;

rb_ret_t rb_init(ring_buffer_t* rb, void* buff, size_t buff_size, size_t el_size);

rb_ret_t rb_add(ring_buffer_t* rb, void* data);

rb_ret_t rb_remove(ring_buffer_t* rb);

bool rb_is_full(ring_buffer_t* rb);

bool rb_is_empty(ring_buffer_t* rb);

rb_ret_t rb_init_read_it(ring_buffer_t* rb, rb_it_t* it);

rb_ret_t rb_get_next_val(rb_it_t* it, void* data_out);

#ifdef __cplusplus
}
#endif

#endif