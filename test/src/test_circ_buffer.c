#include "unity_fixture.h"
#include "circ_buffer.h"

#include <string.h>

enum {TEST_CB_BUFFER_SIZE = 40};
static char buffer[3*TEST_CB_BUFFER_SIZE];
static CB_T cb = {.size = TEST_CB_BUFFER_SIZE, .buffer = buffer + TEST_CB_BUFFER_SIZE};

TEST_GROUP(CircBuffer);

TEST_SETUP(CircBuffer)
{
    memset(buffer, 0xaa, 3*TEST_CB_BUFFER_SIZE);
    cb_init(&cb);
}

TEST_TEAR_DOWN(CircBuffer)
{
    TEST_ASSERT_EACH_EQUAL_HEX8_MESSAGE(0xaa, &buffer[0], TEST_CB_BUFFER_SIZE, "Data overwritten before start of buffer");
    TEST_ASSERT_EACH_EQUAL_HEX8_MESSAGE(0xaa, &buffer[2*TEST_CB_BUFFER_SIZE], TEST_CB_BUFFER_SIZE, "Data overwritten after end of buffer");
}

TEST(CircBuffer, EmptyAfterInit)
{
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(cb.head, cb.tail, "Expected head == tail if empty");
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
    TEST_ASSERT_FALSE(cb_is_full(&cb));
}

TEST(CircBuffer, DequeueNullIfEmpty)
{
    TEST_ASSERT_EQUAL_HEX8_MESSAGE('\0', cb_dequeue(&cb), "Expected NULL after null dequeue");
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
    TEST_ASSERT_FALSE(cb_is_full(&cb));
}

TEST(CircBuffer, QueueDequeueOneCharacter)
{
    for (CB_INDEX_T i = 0; i < cb_capacity(&cb) + 5; ++i)
    {
        cb_queue(&cb,'A' + i);
        TEST_ASSERT_FALSE(cb_is_empty(&cb));
        TEST_ASSERT_EQUAL_HEX8('A' + i, cb_dequeue(&cb));
        TEST_ASSERT_TRUE(cb_is_empty(&cb));
    }
}

TEST(CircBuffer, QueueDequeueMultipleCharacters)
{
    for (CB_INDEX_T i = 0; i < cb_capacity(&cb) - 3; ++i)
    {
        cb_queue(&cb,'A' + i);
        TEST_ASSERT_FALSE(cb_is_full(&cb));
    }
    for (CB_INDEX_T i = 0; i < cb_capacity(&cb) - 3; ++i)
    {
        TEST_ASSERT_FALSE(cb_is_empty(&cb));
        TEST_ASSERT_EQUAL_HEX8('A' + i, cb_dequeue(&cb));
    }    
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
}

TEST(CircBuffer, QueueToCapacity)
{
    for (CB_INDEX_T i = 0; i < cb_capacity(&cb); ++i)
    {
        TEST_ASSERT_FALSE(cb_is_full(&cb));
        cb_queue(&cb,'A' + i);
    }
    TEST_ASSERT_TRUE(cb_is_full(&cb));
    for (CB_INDEX_T i = 0; i < cb_capacity(&cb); ++i)
    {
        TEST_ASSERT_FALSE(cb_is_empty(&cb));
        TEST_ASSERT_EQUAL_HEX8('A' + i, cb_dequeue(&cb));
    }    
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
}

TEST(CircBuffer, QueueToOverflow)
{
    for (CB_INDEX_T i = 0; i < cb_capacity(&cb) + 5; ++i)
    {
        cb_queue(&cb,'A' + i);
    }
    for (CB_INDEX_T i = 0; i < cb_capacity(&cb); ++i)
    {
        TEST_ASSERT_EQUAL_HEX8_MESSAGE('A' + i,cb_dequeue(&cb), "Expected new characters dropped");
    } 
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
}

TEST_GROUP_RUNNER(CircBuffer)
{
    RUN_TEST_CASE(CircBuffer, EmptyAfterInit);
    RUN_TEST_CASE(CircBuffer, DequeueNullIfEmpty);
    RUN_TEST_CASE(CircBuffer, QueueDequeueOneCharacter);
    RUN_TEST_CASE(CircBuffer, QueueDequeueMultipleCharacters);
    RUN_TEST_CASE(CircBuffer, QueueToCapacity);
    RUN_TEST_CASE(CircBuffer, QueueToOverflow);
}
