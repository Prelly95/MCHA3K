#include "unity_fixture.h"
#include "iospy.h"

#include <string.h>
#include "cmd_line_buffer.h"
#include "led.h"

CLB_CREATE_STATIC(clb, 80);

TEST_GROUP(CmdLED);

TEST_SETUP(CmdLED)
{
    clb_init(&clb);
    led_init();
}

TEST_TEAR_DOWN(CmdLED)
{
}

TEST(CmdLED, GetBrightness)
{
    char out[80];

    led_set_brightness(42);

    iospy_hook();
    iospy_push_in_str("led\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("LED brightness is 42\n", out);
}

TEST(CmdLED, SetBrightness)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("led 73\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_UINT8(73, led_get_brightness());
    TEST_ASSERT_EQUAL_STRING("LED brightness set to 73\n",out);
}

TEST(CmdLED, SetBrightnessOverflow)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("led 260\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_UINT8(255, led_get_brightness());
    TEST_ASSERT_EQUAL_STRING("LED brightness set to 255\n",out);
}

TEST(CmdLED, SetBrightnessUnderflow)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("led -5\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_UINT8(0, led_get_brightness());
    TEST_ASSERT_EQUAL_STRING("LED brightness set to 0\n",out);
}

TEST(CmdLED, TurnOn)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("led on\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_UINT8(255, led_get_brightness());
    TEST_ASSERT_EQUAL_STRING("LED is on\n",out);
}

TEST(CmdLED, TurnOff)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("led off\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_UINT8(0, led_get_brightness());
    TEST_ASSERT_EQUAL_STRING("LED is off\n",out);
}

TEST(CmdLED, InvalidArgument)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("led blah\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_UINT8(0, led_get_brightness());
    TEST_ASSERT_EQUAL_STRING("led: invalid argument \"blah\", syntax is: led [on|off|<value>]\n",out);
}

TEST(CmdLED, BlankArgument)
{
    char out[80];

    led_set_brightness(13);

    iospy_hook();
    iospy_push_in_str("led \n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("LED brightness is 13\n",out);
}

TEST_GROUP_RUNNER(CmdLED)
{
    RUN_TEST_CASE(CmdLED, GetBrightness);
    RUN_TEST_CASE(CmdLED, SetBrightness);
    RUN_TEST_CASE(CmdLED, SetBrightnessOverflow);
    RUN_TEST_CASE(CmdLED, SetBrightnessUnderflow);
    RUN_TEST_CASE(CmdLED, TurnOn);
    RUN_TEST_CASE(CmdLED, TurnOff);
    RUN_TEST_CASE(CmdLED, InvalidArgument);
    RUN_TEST_CASE(CmdLED, BlankArgument);
}