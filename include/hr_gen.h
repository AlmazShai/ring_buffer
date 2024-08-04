#ifndef HR_GEN_H
#define HR_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define HR_GEN_MIN_HR 44u
#define HR_GEN_MAX_HR 185u

uint8_t hr_gen_random();

#ifdef __cplusplus
}
#endif

#endif