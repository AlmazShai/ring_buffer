#ifndef HR_EMA_H
#define HR_EMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ring_buffer.h"

/**
 * @brief   The α coefficient in the following formula for calculation of the simple
 *          exponential average:
 *              s(t) = αx(t) + (1-α)st-1
 */
#define HR_EMA_ALPHA 0.8f

/**
 * @brief   Calculates the simple exponential moving average (EMA) of heart rate based on
 *          the data in the specified ring buffer. The HR_EMA_ALPHA coefficient is used
 *          for the calculation.
 *
 * @param rb        Pointer to ring buffer with hear rate data
 * @return uint8_t  Calculated average value
 */
uint8_t hr_ema_calc(ring_buffer_t* rb);

#ifdef __cplusplus
}
#endif

#endif