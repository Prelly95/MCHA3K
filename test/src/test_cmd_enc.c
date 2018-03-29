#include "unity_fixture.h"
#include "stdio_redirect.h"

#include <string.h>
#include "cmd_line_buffer.h"
#include "encoder.h"

CLB_CREATE_STATIC(clb, 80);

TEST_GROUP(CmdEnc);

TEST_SETUP(CmdEnc)
{
    clb_init(&clb);
    encoder_init();
}

TEST_TEAR_DOWN(CmdEnc)
{
}

TEST(CmdEnc, GetCount)
{
    char out[80];

    encoder_set_count(24);

    push_stdio();
    fputs("enc\n",fstdin());
    rewind(fstdin());
    clb_process(&clb);
    if (feof(stdin)) clearerr(stdin); // <-- Secret sauce

    rewind(stdout);
    fgets(out, sizeof(out), fstdout());
    pop_stdio();

    TEST_ASSERT_EQUAL_STRING("Encoder count is 24\n",out);
}

TEST(CmdEnc, ResetCount)
{
    char out[80];

    encoder_set_count(-55);

    push_stdio();
    fputs("enc reset\n",fstdin());
    rewind(fstdin());
    clb_process(&clb);
    if (feof(stdin)) clearerr(stdin); // <-- Secret sauce

    rewind(stdout);
    fgets(out, sizeof(out), fstdout());
    pop_stdio();

    TEST_ASSERT_EQUAL_STRING("Encoder count reset to 0\n",out);
    TEST_ASSERT_EQUAL_INT32(0, encoder_get_count());
}

TEST(CmdEnc, InvalidArgument)
{
    char out[80];

    push_stdio();
    fputs("enc derp\n",fstdin());
    rewind(fstdin());
    clb_process(&clb);
    if (feof(stdin)) clearerr(stdin); // <-- Secret sauce

    rewind(stdout);
    fgets(out, sizeof(out), fstdout());
    pop_stdio();

    TEST_ASSERT_EQUAL_STRING("enc: invalid argument \"derp\", syntax is: enc [reset]\n", out);    
}

TEST_GROUP_RUNNER(CmdEnc)
{
    RUN_TEST_CASE(CmdEnc, GetCount);
    RUN_TEST_CASE(CmdEnc, ResetCount);
    RUN_TEST_CASE(CmdEnc, InvalidArgument);
}