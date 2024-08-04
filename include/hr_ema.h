#ifndef HR_EMA_H
#define HR_EMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ring_buffer.h"

#define HR_EMA_ALPHA 0.8f

uint8_t hr_ema_calc(ring_buffer_t* rb);

#ifdef __cplusplus
}
#endif

#endif