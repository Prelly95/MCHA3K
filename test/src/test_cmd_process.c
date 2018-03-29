#include "unity_fixture.h"
#include "stdio_redirect.h"

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
}

TEST(CmdProcess, BufferEmptyAfterParse)
{
    push_stdio();
    fputs("help\n",fstdin());
    rewind(fstdin());
    clb_process(&clb);
    pop_stdio();

    TEST_ASSERT_TRUE_MESSAGE(clb_is_empty(&clb), "Expected empty command buffer after parsing command");
}

TEST(CmdProcess, InputBufferOverflow)
{
    char out[1024];
    memset(&out, '\0', sizeof(out));

    push_stdio();
    fputs("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n",fstdin());
    rewind(fstdin());
    clb_process(&clb);
    if (feof(stdin)) clearerr(stdin); // <-- Secret sauce

    rewind(stdout);
    fgets(out, sizeof(out), fstdout());
    pop_stdio();

    TEST_ASSERT_EQUAL_STRING("*** Max command length exceeded ***\n", out);
}

TEST(CmdProcess, SilentIfIncomplete)
{
    char out[80];

    push_stdio();
    fputs("hek\bl",fstdin());
    fflush(fstdin());
    rewind(fstdin());
    clb_process(&clb);
    if (feof(stdin)) clearerr(stdin); // <-- Secret sauce

    rewind(stdout);
    char * s = fgets(out, sizeof(out), fstdout());
    int is_eof = feof(fstdout());
    pop_stdio();

    TEST_ASSERT_EQUAL_STRING(NULL, s);
    TEST_ASSERT_TRUE(is_eof);
}

TEST_GROUP_RUNNER(CmdProcess)
{
    RUN_TEST_CASE(CmdProcess, BufferEmptyAfterParse);
    RUN_TEST_CASE(CmdProcess, InputBufferOverflow);
    RUN_TEST_CASE(CmdProcess, SilentIfIncomplete);
}