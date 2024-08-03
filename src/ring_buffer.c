#include "ring_buffer.h"

#include <cstring>

#define CHECK_IF_INIT(rb)                                                                \
    if ((rb->buff == NULL) || (rb->cap == 0) || (rb->el_size == 0))                      \
    return RB_NOT_INIT

// increments the index of the ring buffer and moves it to the beginning if the index has
// reached the end of the buffer
static void increment_idx(ring_buffer_t* rb, size_t* const idx)
{
    (*idx)++;
    if (*idx >= rb->cap)
    {
        (*idx) = 0;
    }
}

rb_ret_t rb_init(ring_buffer_t* rb, void* buff, size_t buff_size, size_t el_size)
{
    if ((rb == NULL) || (buff == NULL) || (el_size == 0) || (buff_size < el_size))
    {
        return RB_INVALID_ARG;
    }

    rb->buff    = buff;
    rb->cap     = buff_size / el_size;
    rb->el_size = el_size;
    rb->head    = 0;
    rb->tail    = 0;
    return RB_OK;
}

rb_ret_t rb_add(ring_buffer_t* rb, void* data)
{
    CHECK_IF_INIT(rb);

    if (rb_is_full(rb) == true)
    {
        return RB_FULL;
    }

    memcpy((char*)rb->buff + (rb->head * rb->el_size), data, rb->el_size);

    increment_idx(rb, &rb->head);
    return RB_OK;
}

rb_ret_t rb_remove(ring_buffer_t* rb)
{
    CHECK_IF_INIT(rb);

    if (rb_is_empty(rb) == true)
    {
        return RB_EMPTY;
    }

    increment_idx(rb, &rb->tail);
    return RB_OK;
}

bool rb_is_full(ring_buffer_t* rb)
{
    size_t nextHead = rb->head;
    increment_idx(rb, &nextHead);
    return (nextHead == rb->tail);
}

bool rb_is_empty(ring_buffer_t* rb)
{
    return rb->tail == rb->head;
}

rb_ret_t rb_init_read_it(ring_buffer_t* rb, rb_it_t* it)
{
    CHECK_IF_INIT(rb);

    if (it == NULL)
    {
        return RB_INVALID_ARG;
    }

    it->rb  = rb;
    it->idx = rb->tail;
    return RB_OK;
}

rb_ret_t rb_get_next_val(rb_it_t* it, void* data_out)
{
    if (it->rb == NULL)
    {
        return RB_NOT_INIT;
    }

    if (it->idx == it->rb->head)
    {
        return RB_EMPTY;
    }

    memcpy(data_out, (char*)it->rb->buff + (it->idx * it->rb->el_size), it->rb->el_size);
    increment_idx(it->rb, &it->idx);
    return RB_OK;
}