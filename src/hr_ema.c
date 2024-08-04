#include "hr_ema.h"

#include <math.h>

uint8_t hr_ema_calc(ring_buffer_t* rb)
{
    rb_it_t it;
    if (rb_init_read_it(rb, &it) != RB_OK)
    {
        return 0;
    }

    uint8_t val;
    float   res = 0;
    // initialize first value
    if (rb_get_next_val(&it, &val) == RB_OK)
    {
        res = val;
    }

    // calculate exponential smoothing based on rest elements using following formula:
    // s(t) = αx(t) + (1-α)st-1
    while (rb_get_next_val(&it, &val) == RB_OK)
    {
        res = HR_EMA_ALPHA * val + (1 - HR_EMA_ALPHA) * res;
    }
    return (uint8_t)round(res);
}