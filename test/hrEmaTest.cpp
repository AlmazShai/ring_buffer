#include "gtest/gtest.h"

#include "hr_ema.h"

#include <list>

namespace
{
TEST(hrEmaTest, hr_ema_calc_GivenNotFullBuffer_CalculatesCorrectEma)
{
    // Arrange
    std::list<uint8_t> givenHrs = {65, 50, 75};

    const size_t elSize   = sizeof(uint8_t);
    const size_t cap      = 10;
    const size_t buffSize = (cap + 1) * elSize;
    char         buff[buffSize];

    ring_buffer_t rb;
    ASSERT_EQ(rb_init(&rb, buff, buffSize, elSize), RB_OK);

    for (auto val: givenHrs)
    {
        ASSERT_EQ(rb_add(&rb, &val), RB_OK);
    }

    // Act
    uint8_t res = hr_ema_calc(&rb);

    // Assert
    uint8_t expectedEma = 71; // 70.6 rounded up to 71
    EXPECT_EQ(res, expectedEma);
}

TEST(hrEmaTest, hr_ema_calc_GivenNotFullBufferWithOverwrittenValues_CalculatesCorrectEma)
{ // Arrange
    std::list<uint8_t> givenHrs = {65, 50, 75, 100, 85};

    const size_t elSize   = sizeof(uint8_t);
    const size_t cap      = 5;
    const size_t buffSize = (cap + 1) * elSize;
    char         buff[buffSize];

    ring_buffer_t rb;
    ASSERT_EQ(rb_init(&rb, buff, buffSize, elSize), RB_OK);

    // fill buffer with given values
    for (auto val: givenHrs)
    {
        ASSERT_EQ(rb_add(&rb, &val), RB_OK);
    }

    // add two new values
    ASSERT_EQ(rb_remove(&rb), RB_OK);
    ASSERT_EQ(rb_remove(&rb), RB_OK);

    uint8_t val1 = 66;
    ASSERT_EQ(rb_add(&rb, &val1), RB_OK);
    givenHrs.push_back(val1);

    uint8_t val2 = 130;
    ASSERT_EQ(rb_add(&rb, &val2), RB_OK);
    givenHrs.push_back(val2);

    // Act
    uint8_t res = hr_ema_calc(&rb);

    // Assert
    // Based on input: [75, 100, 85, 66, 130] expectedEma is 118.05 => 118
    uint8_t expectedEma = 118;
    EXPECT_EQ(res, expectedEma);
}
} // namespace