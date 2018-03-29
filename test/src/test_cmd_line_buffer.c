#include "unity_fixture.h"
#include "cmd_line_buffer.h"

#include <string.h>

enum {TEST_CLB_BUFFER_SIZE = 40};
static char buffer[3*TEST_CLB_BUFFER_SIZE];
static CLB_T clb = {.size = TEST_CLB_BUFFER_SIZE, .buffer = buffer + TEST_CLB_BUFFER_SIZE};

TEST_GROUP(CmdLineBuffer);

TEST_SETUP(CmdLineBuffer)
{
    memset(buffer, 0xaa, 3*TEST_CLB_BUFFER_SIZE);
    clb_init(&clb);
}

TEST_TEAR_DOWN(CmdLineBuffer)
{
    TEST_ASSERT_EACH_EQUAL_HEX8_MESSAGE(0xaa, &buffer[0], TEST_CLB_BUFFER_SIZE, "Data overwritten before start of buffer");
    TEST_ASSERT_EACH_EQUAL_HEX8_MESSAGE(0xaa, &buffer[2*TEST_CLB_BUFFER_SIZE], TEST_CLB_BUFFER_SIZE, "Data overwritten after end of buffer");
}

TEST(CmdLineBuffer, EmptyAfterInit)
{
    TEST_ASSERT_EQUAL_UINT16(0, clb.count);
    TEST_ASSERT_TRUE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_FALSE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_NULL(clb_gets(&clb));
}

TEST(CmdLineBuffer, CharLF)
{
    TEST_ASSERT_EQUAL_HEX(CLB_SUCCESS, clb_consume_char(&clb,'A'));
    TEST_ASSERT_EQUAL_HEX(CLB_SUCCESS, clb_consume_char(&clb,'B'));
    TEST_ASSERT_EQUAL_HEX(CLB_SUCCESS, clb_consume_char(&clb,'C'));
    TEST_ASSERT_EQUAL_HEX(CLB_CMD_READY, clb_consume_char(&clb,'\n'));
    TEST_ASSERT_FALSE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_TRUE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_EQUAL_STRING("ABC", clb_gets(&clb));
}

TEST(CmdLineBuffer, StringLF)
{
    const char * str = "String\n";
    TEST_ASSERT_EQUAL_HEX(CLB_CMD_READY, clb_consume_str(&clb,str));
    TEST_ASSERT_FALSE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_TRUE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_EQUAL_STRING("String", clb_gets(&clb));
}

TEST(CmdLineBuffer, StringCRLF)
{
    const char * str = "String\r\n";
    TEST_ASSERT_EQUAL_HEX(CLB_CMD_READY, clb_consume_str(&clb,str));
    TEST_ASSERT_FALSE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_TRUE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_EQUAL_STRING("String", clb_gets(&clb));
}

TEST(CmdLineBuffer, StringBackspace)
{
    const char * str = "SA\btB\brC\biD\bnE\bgF\bString\b\b\b\b\b\b\n";
    TEST_ASSERT_EQUAL_HEX(CLB_CMD_READY, clb_consume_str(&clb,str));
    TEST_ASSERT_FALSE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_TRUE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_EQUAL_STRING("String", clb_gets(&clb));
}

TEST(CmdLineBuffer, EmptyLineLF)
{
    const char * str = "\n";
    TEST_ASSERT_EQUAL_HEX(CLB_SUCCESS, clb_consume_str(&clb, str));
    TEST_ASSERT_TRUE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_FALSE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_NULL(clb_gets(&clb));
}

TEST(CmdLineBuffer, EmptyBackspace)
{
    const char * str = "Z\b\b\b\n";
    TEST_ASSERT_EQUAL_HEX(CLB_SUCCESS, clb_consume_str(&clb, str));
    TEST_ASSERT_TRUE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_FALSE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_NULL(clb_gets(&clb));
}

TEST(CmdLineBuffer, Unterminated)
{
    const char * str = "String";
    TEST_ASSERT_EQUAL_HEX(CLB_SUCCESS, clb_consume_str(&clb, str));
    TEST_ASSERT_FALSE(clb_is_empty(&clb));
    TEST_ASSERT_FALSE(clb_is_full(&clb));
    TEST_ASSERT_FALSE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_NULL(clb_gets(&clb));
}

TEST(CmdLineBuffer, CharOverflow)
{
    for (int i = 0; i < clb.size; ++i)
    {
        TEST_ASSERT_EQUAL_HEX(CLB_SUCCESS, clb_consume_char(&clb,'A' + i));
    }
    TEST_ASSERT_EQUAL_HEX(CLB_BUFFER_FULL, clb_consume_char(&clb,'\n'));
    TEST_ASSERT_FALSE(clb_is_empty(&clb));
    TEST_ASSERT_TRUE(clb_is_full(&clb));
    TEST_ASSERT_FALSE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_NULL(clb_gets(&clb));
}

TEST(CmdLineBuffer, StringOverflow)
{
    const char * str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\n";
    TEST_ASSERT_EQUAL_HEX(CLB_BUFFER_FULL, clb_consume_str(&clb,str));
    TEST_ASSERT_FALSE(clb_is_empty(&clb));
    TEST_ASSERT_TRUE(clb_is_full(&clb));
    TEST_ASSERT_FALSE(clb_is_cmd_ready(&clb));
    TEST_ASSERT_NULL(clb_gets(&clb));
}

TEST_GROUP_RUNNER(CmdLineBuffer)
{
    RUN_TEST_CASE(CmdLineBuffer, EmptyAfterInit);
    RUN_TEST_CASE(CmdLineBuffer, CharLF);
    RUN_TEST_CASE(CmdLineBuffer, StringLF);
    RUN_TEST_CASE(CmdLineBuffer, StringCRLF);
    RUN_TEST_CASE(CmdLineBuffer, StringBackspace);
    RUN_TEST_CASE(CmdLineBuffer, EmptyLineLF);
    RUN_TEST_CASE(CmdLineBuffer, EmptyBackspace);
    RUN_TEST_CASE(CmdLineBuffer, Unterminated);
    RUN_TEST_CASE(CmdLineBuffer, CharOverflow);
    RUN_TEST_CASE(CmdLineBuffer, StringOverflow);
}