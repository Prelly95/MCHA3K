#include "unity_fixture.h"
#include "iospy.h"

#include <string.h>
#include "cmd_line_buffer.h"

CLB_CREATE_STATIC(clb, 20);

TEST_GROUP(CmdProcess);

TEST_SETUP(CmdProcess)
{
    clb_init(&clb);
}

TEST_TEAR_DOWN(CmdProcess)
{
    iospy_unhook();
}

TEST(CmdProcess, BufferEmptyAfterParse)
{
    iospy_hook();
    iospy_push_in_str("help\n");
    clb_process(&clb);
    iospy_unhook();

    TEST_ASSERT_TRUE_MESSAGE(clb_is_empty(&clb), "Expected empty command buffer after parsing command");
}

TEST(CmdProcess, InputBufferOverflow)
{
    char out[1024];

    iospy_hook();
    iospy_push_in_str("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("*** Max command length exceeded ***\n", out);
}

TEST(CmdProcess, SilentIfIncomplete)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("hek\bl");
    clb_process(&clb);
    size_t n = iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_UINT(0, n);
    TEST_ASSERT_EQUAL_STRING("", out);
}

#ifdef NO_LD_WRAP
void __wrap_cmd_parse(char *) __asm__("_cmd_parse");
#endif

void __real_cmd_parse(char *);

void __wrap_cmd_parse(char *s)
{
    if (s && !strcmp(s, "testcmd"))
    {
        printf(
            "\n"
            "This\n"
            "is\n"
            "a\n"
            "test\n"
            "\n"
        );        
    }
    else
    {
        __real_cmd_parse(s);
    }
}

TEST(CmdProcess, TestCmd)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("tek\bstcmd\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("\nThis\nis\na\ntest\n\n", out);
}

TEST(CmdProcess, TestCmdTwice)
{
    char out1[256], out2[256];
    
    iospy_hook();

    iospy_push_in_str("tek\bstcmd\n");
    clb_process(&clb);
    iospy_pop_out_str(out1, sizeof(out1));

    iospy_push_in_str("testcmd\n");
    clb_process(&clb);
    iospy_pop_out_str(out2, sizeof(out2));

    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("\nThis\nis\na\ntest\n\n", out1);
    TEST_ASSERT_EQUAL_STRING("\nThis\nis\na\ntest\n\n", out2);
}

TEST(CmdProcess, TestCmdInterrupted)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("tes");
    clb_process(&clb);
    iospy_push_in_str("tcmd\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("\nThis\nis\na\ntest\n\n", out);
}

TEST_GROUP_RUNNER(CmdProcess)
{
    RUN_TEST_CASE(CmdProcess, BufferEmptyAfterParse);
    RUN_TEST_CASE(CmdProcess, InputBufferOverflow);
    RUN_TEST_CASE(CmdProcess, SilentIfIncomplete);
    RUN_TEST_CASE(CmdProcess, TestCmd);
    RUN_TEST_CASE(CmdProcess, TestCmdTwice);
    RUN_TEST_CASE(CmdProcess, TestCmdInterrupted);
}