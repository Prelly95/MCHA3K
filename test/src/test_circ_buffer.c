#include "unity_fixture.h"
#include "circ_buffer.h"

#include <string.h>


static char buffer[18];
static CB_T cb = {.size = 16, .buffer = buffer + 1};

TEST_GROUP(CircBuffer);

TEST_SETUP(CircBuffer)
{
    memset(buffer, 0xaa, 18);
    cb_init(&cb);
}

TEST_TEAR_DOWN(CircBuffer)
{
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xaa,buffer[0],"Data overwritten before start of buffer");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xaa,buffer[17],"Data overwritten after end of buffer");    
}

TEST(CircBuffer,EmptyAfterInit)
{
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(cb.head, cb.tail,"Expected head == tail if empty");
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
    TEST_ASSERT_FALSE(cb_is_full(&cb));
}

TEST(CircBuffer,DequeueNullIfEmpty)
{
    TEST_ASSERT_EQUAL_HEX8_MESSAGE('\0',cb_dequeue(&cb),"Expected NULL after null dequeue");
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
    TEST_ASSERT_FALSE(cb_is_full(&cb));
}

TEST(CircBuffer,QueueDequeueOneCharacter)
{
    for (int i = 0; i < cb.size + 4; ++i)
    {
        cb_queue(&cb,'A' + i);
        TEST_ASSERT_FALSE(cb_is_empty(&cb));
        TEST_ASSERT_EQUAL_HEX8('A' + i,cb_dequeue(&cb));
        TEST_ASSERT_TRUE(cb_is_empty(&cb));
    }
}

TEST(CircBuffer,QueueDequeueMultipleCharacters)
{
    for (int i = 0; i < cb.size - 4; ++i)
    {
        cb_queue(&cb,'A' + i);
        TEST_ASSERT_FALSE(cb_is_full(&cb));
    }
    for (int i = 0; i < cb.size - 4; ++i)
    {
        TEST_ASSERT_FALSE(cb_is_empty(&cb));
        TEST_ASSERT_EQUAL_HEX8('A' + i,cb_dequeue(&cb));
    }    
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
}

TEST(CircBuffer,QueueToCapacity)
{
    for (int i = 0; i < cb.size - 1; ++i)
    {
        TEST_ASSERT_FALSE(cb_is_full(&cb));
        cb_queue(&cb,'A' + i);
    }
    TEST_ASSERT_TRUE(cb_is_full(&cb));
    for (int i = 0; i < cb.size - 1; ++i)   // useful capacity is size - 1
    {
        TEST_ASSERT_FALSE(cb_is_empty(&cb));
        TEST_ASSERT_EQUAL_HEX8('A' + i,cb_dequeue(&cb));
    }    
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
}

TEST(CircBuffer,QueueToOverflow)
{
    // cb_init(&cb);
    for (int i = 0; i < cb.size + 4; ++i)
    {
        cb_queue(&cb,'A' + i);
    }
    for (int i = 0; i < cb.size - 1; ++i)   // useful capacity is size - 1
    {
        TEST_ASSERT_EQUAL_HEX8_MESSAGE('A' + i,cb_dequeue(&cb),"Expected new characters dropped");
    } 
    TEST_ASSERT_TRUE(cb_is_empty(&cb));
}

TEST_GROUP_RUNNER(CircBuffer)
{
    RUN_TEST_CASE(CircBuffer,EmptyAfterInit);
    RUN_TEST_CASE(CircBuffer,DequeueNullIfEmpty);
    RUN_TEST_CASE(CircBuffer,QueueDequeueOneCharacter);
    RUN_TEST_CASE(CircBuffer,QueueDequeueMultipleCharacters);
    RUN_TEST_CASE(CircBuffer,QueueToCapacity);
    RUN_TEST_CASE(CircBuffer,QueueToOverflow);
}
