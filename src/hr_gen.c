#include "hr_gen.h"

#include <stdlib.h>

uint8_t hr_gen_random()
{
    return (rand() % (HR_GEN_MAX_HR - HR_GEN_MIN_HR + 1)) + HR_GEN_MIN_HR;
}