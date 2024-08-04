
#include "gtest/gtest.h"

#include "ring_buffer.h"

namespace
{

class RingBufferTest : public ::testing::Test
{
protected:

    ring_buffer_t m_rb;
};

class RingBufferInitialized : public RingBufferTest
{
public:

    void SetUp() override
    {
        m_buff = malloc(m_buffSize);
        ASSERT_EQ(rb_init(&m_rb, m_buff, m_buffSize, m_elSize), RB_OK);
    }

    void TearDown() override
    {
        free(m_buff);
    }

protected:

    const uint32_t m_elSize   = sizeof(size_t);
    const size_t   m_cap      = 5;
    const size_t   m_buffSize = m_elSize * (m_cap + 1);
    void*          m_buff;
};

class RingBufferFull : public RingBufferInitialized
{
public:

    void SetUp() override
    {
        RingBufferInitialized::SetUp();

        for (size_t i = 0; i < m_cap; ++i)
        {
            ASSERT_EQ(rb_add(&m_rb, &i), RB_OK);
        }
    }

    void TearDown() override
    {
        RingBufferInitialized::TearDown();
    }
};

TEST_F(RingBufferTest, rb_init_WhenGivenInvalidArgument_ReturnsError)
{
    // Arrange
    ring_buffer_t rb       = {};
    size_t        elSize   = sizeof(int);
    size_t        buffSize = elSize * 5;
    char          buff[buffSize];

    // Act & assert
    EXPECT_EQ(rb_init(NULL, buff, buffSize, elSize), RB_INVALID_ARG);
    EXPECT_EQ(rb_init(&rb, NULL, buffSize, elSize), RB_INVALID_ARG);
    EXPECT_EQ(rb_init(&rb, &buff, 0, elSize), RB_INVALID_ARG);
    EXPECT_EQ(rb_init(&rb, &buff, buffSize, 0), RB_INVALID_ARG);
}

TEST_F(RingBufferTest, rb_add_WhenNotInitialized_ReturnsError)
{
    // Arrange
    ring_buffer_t rb = {};
    uint32_t      val;

    // Act & assert
    EXPECT_EQ(rb_add(&rb, &val), RB_NOT_INIT);
}

TEST_F(RingBufferFull, rb_add_GivenFullBuffer_WhenAddOneElement_ReturnsError)
{
    int val = 3;
    EXPECT_EQ(rb_add(&m_rb, &val), RB_FULL);
}

TEST_F(RingBufferFull, rb_add_GivenFullBuffer_WhenRemoveOneElementAndAddOne_Succeeds)
{
    int val = 3;
    EXPECT_EQ(rb_remove(&m_rb), RB_OK);
    EXPECT_EQ(rb_add(&m_rb, &val), RB_OK);
    EXPECT_EQ(rb_add(&m_rb, &val), RB_FULL);
}

TEST_F(RingBufferInitialized, rb_add_GivenEmptyBuffer_WhenAddMaxElements_Succeeds)
{
    for (size_t i = 0; i < m_cap; ++i)
    {
        EXPECT_EQ(rb_add(&m_rb, &i), RB_OK);
    }
}

TEST_F(RingBufferTest, rb_remove_WhenNotInitialized_ReturnsError)
{
    ring_buffer_t rb = {};
    EXPECT_EQ(rb_remove(&rb), RB_NOT_INIT);
}

TEST_F(RingBufferInitialized, rb_remove_GivenEmptyBuffer_ReturnsError)
{
    EXPECT_EQ(rb_remove(&m_rb), RB_EMPTY);
}

TEST_F(RingBufferInitialized,
       rb_remove_GivenOneElement_WhenRemoveTwoElements_ReturnsErrorOnSecond)
{
    // Arrange
    size_t val;
    EXPECT_EQ(rb_add(&m_rb, &val), RB_OK);

    // Act & assert
    EXPECT_EQ(rb_remove(&m_rb), RB_OK);
    EXPECT_EQ(rb_remove(&m_rb), RB_EMPTY);
}

TEST_F(RingBufferFull, rb_remove_GivenFullBuffer_WhenRemoveAllElements_Succeed)
{
    for (size_t i = 0; i < m_cap; ++i)
    {
        EXPECT_EQ(rb_remove(&m_rb), RB_OK);
    }
    EXPECT_EQ(rb_remove(&m_rb), RB_EMPTY);
}

TEST_F(RingBufferInitialized, rb_is_full_GivenEmptyBuffer_ReturnsFalse)
{
    EXPECT_FALSE(rb_is_full(&m_rb));
}

TEST_F(RingBufferFull, rb_is_full_GivenFullBuffer_ReturnsTrue)
{
    EXPECT_TRUE(rb_is_full(&m_rb));
}

TEST_F(RingBufferFull, rb_is_full_WhenTailAtZeroAndHeadInTheEnd_ReturnsTrue)
{
    EXPECT_EQ(m_rb.tail, 0);
    EXPECT_EQ(m_rb.head, m_cap);

    EXPECT_TRUE(rb_is_full(&m_rb));
}

TEST_F(RingBufferFull, rb_is_full_WhenHeadAtZeroAndTailInTheEnd_ReturnsFalse)
{
    while (rb_remove(&m_rb) == RB_OK)
    {}
    size_t val = 124;
    EXPECT_EQ(rb_add(&m_rb, &val), RB_OK);

    EXPECT_EQ(m_rb.head, 0);
    EXPECT_EQ(m_rb.tail, m_cap);

    // Act & assert
    EXPECT_FALSE(rb_is_full(&m_rb));
}

TEST_F(RingBufferInitialized, rb_is_empty_GivenBufferWithOneElement_ReturnsFalse)
{
    size_t val = 1;
    EXPECT_EQ(rb_add(&m_rb, &val), RB_OK);

    EXPECT_FALSE(rb_is_empty(&m_rb));
}

TEST_F(RingBufferInitialized, rb_is_empty_GivenEmptyBuffer_ReturnsTrue)
{
    EXPECT_TRUE(rb_is_empty(&m_rb));
}

TEST_F(RingBufferFull, rb_is_empty_GivenFullBuffer_ReturnsFalse)
{
    EXPECT_FALSE(rb_is_empty(&m_rb));
}

TEST_F(RingBufferInitialized,
       rb_is_empty_GivenBufferIsNotEmpty_WhenRemoveAllElements_ReturnsTrue)
{
    // add and remove capacity /2 number of element
    for (size_t i = 0; i < m_cap / 2; ++i)
    {
        EXPECT_EQ(rb_add(&m_rb, &i), RB_OK);
    }
    for (size_t i = 0; i < m_cap / 2; ++i)
    {
        EXPECT_EQ(rb_remove(&m_rb), RB_OK);
    }

    EXPECT_TRUE(rb_is_empty(&m_rb));
}

TEST_F(RingBufferFull, rb_is_empty_GivenTailAtZeroAndHeadInTheEnd_ReturnsFalse)
{
    EXPECT_EQ(m_rb.tail, 0);
    EXPECT_EQ(m_rb.head, m_cap);

    EXPECT_FALSE(rb_is_empty(&m_rb));
}

TEST_F(RingBufferFull, rb_is_empty_GivenHeadAtZeroAndTailInTheEnd_ReturnsFalse)
{
    while (rb_remove(&m_rb) == RB_OK)
    {}
    size_t val = 1234;
    EXPECT_EQ(rb_add(&m_rb, &val), RB_OK);

    EXPECT_EQ(m_rb.head, 0);
    EXPECT_EQ(m_rb.tail, m_cap);

    EXPECT_FALSE(rb_is_empty(&m_rb));
}

TEST_F(RingBufferTest, rb_init_read_it_WhenPassedInvalidArguments_ReturnsError)
{
    EXPECT_EQ(rb_init_read_it(&m_rb, NULL), RB_INVALID_ARG);
}

TEST_F(RingBufferFull, rb_init_read_it_WhenPassedValidArguments_IndexEqualToTail)
{
    // make tail index non zero
    EXPECT_EQ(rb_remove(&m_rb), RB_OK);

    // Act
    rb_it_t it = {};
    EXPECT_EQ(rb_init_read_it(&m_rb, &it), RB_OK);

    // Assert
    EXPECT_EQ(it.idx, m_rb.tail);
}

TEST_F(RingBufferInitialized, rb_get_next_val_GivenEmptyBuffer_ReturnError)
{
    rb_it_t it = {};
    ASSERT_EQ(rb_init_read_it(&m_rb, &it), RB_OK);

    size_t val;
    EXPECT_EQ(rb_get_next_val(&it, &val), RB_EMPTY);
}

TEST_F(RingBufferInitialized, rb_get_next_val_WhenNotInitialized_ReturnsError)
{
    rb_it_t it = {};
    size_t  val;
    EXPECT_EQ(rb_get_next_val(&it, &val), RB_NOT_INIT);
}

TEST_F(RingBufferInitialized,
       rb_get_next_val_GivenTwoElements_WhenReadThreeElements_ReturnErrorOnThird)
{
    // Arrange
    size_t val1 = 12;
    size_t val2 = 32;
    EXPECT_EQ(rb_add(&m_rb, &val1), RB_OK);
    EXPECT_EQ(rb_add(&m_rb, &val2), RB_OK);

    rb_it_t it = {};
    ASSERT_EQ(rb_init_read_it(&m_rb, &it), RB_OK);

    // Act
    size_t val;

    EXPECT_EQ(rb_get_next_val(&it, &val), RB_OK);
    EXPECT_EQ(rb_get_next_val(&it, &val), RB_OK);
    EXPECT_EQ(rb_get_next_val(&it, &val), RB_EMPTY);
}

TEST_F(RingBufferFull,
       rb_get_next_val_GivenFullBuffer_WhenRemoveAndAddSomeElements_ReadsCorrectValues)
{
    // Originally buffer is initialized with values from 0 to cap - 1
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);
    // now buffer consist of: [2,3,4]
    // Add one more element
    size_t val = 7;
    ASSERT_EQ(rb_add(&m_rb, &val), RB_OK);

    // Now buffer should consist of following elements:
    size_t expectedValues[] = {2, 3, 4, 7};
    size_t expectedCount    = sizeof(expectedValues) / sizeof(expectedValues[0]);

    // Act & assert
    rb_it_t it = {};
    ASSERT_EQ(rb_init_read_it(&m_rb, &it), RB_OK);
    for (size_t i = 0; i < expectedCount; ++i)
    {
        size_t val;
        ASSERT_EQ(rb_get_next_val(&it, &val), RB_OK);
        EXPECT_EQ(val, expectedValues[i]);
    }

    EXPECT_EQ(rb_get_next_val(&it, NULL), RB_EMPTY);
}

TEST_F(RingBufferInitialized,
       rb_get_next_val_GivenTwoElements_WhenRemoveTwoElements_ReturnsError)
{
    size_t val = 12;
    ASSERT_EQ(rb_add(&m_rb, &val), RB_OK);
    ASSERT_EQ(rb_add(&m_rb, &val), RB_OK);

    ASSERT_EQ(rb_remove(&m_rb), RB_OK);
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);

    // Act & assert
    rb_it_t it = {};
    ASSERT_EQ(rb_init_read_it(&m_rb, &it), RB_OK);
    EXPECT_EQ(rb_get_next_val(&it, &val), RB_EMPTY);
}

TEST_F(RingBufferFull,
       rb_get_next_val_GivenHeaderAtZeroAndBufferIsNotEmpty_ReadsCorrectValues)
{
    // initial values [0,1,2,3,4]
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);

    size_t val = 7;
    ASSERT_EQ(rb_add(&m_rb, &val), RB_OK);

    size_t expectedValues[] = {3, 4, 7};
    size_t expectedCount    = sizeof(expectedValues) / sizeof(expectedValues[0]);

    ASSERT_EQ(m_rb.head, 0);
    ASSERT_NE(m_rb.tail, 0);

    // Act & assert
    rb_it_t it = {};
    ASSERT_EQ(rb_init_read_it(&m_rb, &it), RB_OK);
    for (size_t i = 0; i < expectedCount; ++i)
    {
        size_t val;
        ASSERT_EQ(rb_get_next_val(&it, &val), RB_OK);
        EXPECT_EQ(val, expectedValues[i]);
    }

    EXPECT_EQ(rb_get_next_val(&it, NULL), RB_EMPTY);
}

TEST_F(RingBufferFull,
       rb_get_next_val_GivenHeaderAtFirstIndexAndBufferIsNotEmpty_ReadsCorrectValues)
{
    // initial values [0,1,2,3,4]
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);
    ASSERT_EQ(rb_remove(&m_rb), RB_OK);

    size_t val = 7;
    ASSERT_EQ(rb_add(&m_rb, &val), RB_OK);
    val = 9;
    ASSERT_EQ(rb_add(&m_rb, &val), RB_OK);

    size_t expectedValues[] = {3, 4, 7, 9};
    size_t expectedCount    = sizeof(expectedValues) / sizeof(expectedValues[0]);

    ASSERT_EQ(m_rb.head, 1);
    ASSERT_NE(m_rb.tail, 0);

    // Act & assert
    rb_it_t it = {};
    ASSERT_EQ(rb_init_read_it(&m_rb, &it), RB_OK);
    for (size_t i = 0; i < expectedCount; ++i)
    {
        size_t val;
        ASSERT_EQ(rb_get_next_val(&it, &val), RB_OK);
        EXPECT_EQ(val, expectedValues[i]);
    }

    EXPECT_EQ(rb_get_next_val(&it, NULL), RB_EMPTY);
}

TEST_F(RingBufferTest, rb_get_next_val_WhenReadComplexStructure_ReadCorrectValue)
{
    struct Test
    {
        uint32_t val1;
        uint8_t  val2;
        uint32_t val3;

        bool operator==(const Test& other) const
        {
            return (other.val1 == val1) && (other.val2 == val2) && (other.val3 == val3);
        }
    };

    const size_t elSize   = sizeof(Test);
    const size_t cap      = 5;
    const size_t buffSize = (cap + 1) * elSize;
    char         buff[buffSize];

    ring_buffer_t rb;
    ASSERT_EQ(rb_init(&rb, buff, buffSize, elSize), RB_OK);

    Test expectedEl1 = {123, 8, 32};
    Test expectedEl2 = {321, 6, 23};

    ASSERT_EQ(rb_add(&rb, &expectedEl1), RB_OK);
    ASSERT_EQ(rb_add(&rb, &expectedEl2), RB_OK);

    // Act
    rb_it_t it;
    ASSERT_EQ(rb_init_read_it(&rb, &it), RB_OK);

    Test el1;
    Test el2;
    EXPECT_EQ(rb_get_next_val(&it, &el1), RB_OK);
    EXPECT_EQ(rb_get_next_val(&it, &el2), RB_OK);

    EXPECT_EQ(el1, expectedEl1);
    EXPECT_EQ(el2, expectedEl2);
}

} // namespace