#include "gtest/gtest.h"

#include "hr_gen.h"

namespace
{
TEST(hrGenTest, hr_gen_random_GeneratesWithinGivenRange)
{
    size_t nToGenerate = 500;
    for (size_t i = 0; i < nToGenerate; ++i)
    {
        uint8_t value = hr_gen_random();
        EXPECT_GE(value, HR_GEN_MIN_HR);
        EXPECT_LE(value, HR_GEN_MAX_HR);
    }
}
} // namespace