#include "unity_fixture.h"
#include "iospy.h"

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

    iospy_hook();
    iospy_push_in_str("enc\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("Encoder count is 24\n",out);
}

TEST(CmdEnc, ResetCount)
{
    char out[80];

    encoder_set_count(-55);

    iospy_hook();
    iospy_push_in_str("enc reset\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("Encoder count reset to 0\n",out);
    TEST_ASSERT_EQUAL_INT32(0, encoder_get_count());
}

TEST(CmdEnc, InvalidArgument)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("enc derp\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("enc: invalid argument \"derp\", syntax is: enc [reset]\n", out);    
}

TEST_GROUP_RUNNER(CmdEnc)
{
    RUN_TEST_CASE(CmdEnc, GetCount);
    RUN_TEST_CASE(CmdEnc, ResetCount);
    RUN_TEST_CASE(CmdEnc, InvalidArgument);
}