#include "unity_fixture.h"
#include "iospy.h"

#include <stdint.h>
#include "cmd_line_buffer.h"

static void _given(const char *, float);
static void _expect(const char *, float);

CLB_CREATE_STATIC(clb, 80);

TEST_GROUP(CmdXY);

TEST_SETUP(CmdXY)
{
    clb_init(&clb);
}

TEST_TEAR_DOWN(CmdXY)
{
}

void _given(const char *s, float val)
{
    char in[128];
    sprintf(in, "set %s %g\n", s, val);
    char out[80];

    iospy_hook();
    iospy_push_in_str(in);
    clb_process(&clb);
    size_t n = iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(0, n, "Expected no output");
}

void _expect(const char *s, float val_expected)
{
    char in[128];
    sprintf(in, "get %s\n", s);
    char out[80];

    iospy_hook();
    iospy_push_in_str(in);
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    // Expected reply format
    char fmt[128];
    sprintf(fmt, "%s is %%g\n", s);

    // Match within floating point tolerance
    float val_actual;
    char msg[128];
    sprintf(msg, "Attempted to scan '%s' using format '%s'", out, fmt);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, sscanf(out, fmt, &val_actual), msg);
    TEST_ASSERT_EQUAL_FLOAT(val_expected, val_actual);
}

TEST(CmdXY, InitialXZero)
{
    _expect("x", 0.0f);
}

TEST(CmdXY, InitialYZero)
{
    _expect("y", 0.0f);
}

TEST(CmdXY, SetGetX)
{
    _given("x", 3.4f);
    _expect("x", 3.4f);
}

TEST(CmdXY, SetGetY)
{
    _given("y", 4.9f);
    _expect("y", 4.9f);
}

TEST(CmdXY, MultiplyXY)
{
    float x = 7.3f, y = 1.2f;
    float xy_expected = 8.76f; // x*y

    _given("x", x);
    _given("y", y);

    char out[80];

    iospy_hook();
    iospy_push_in_str("mulxy\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    // Expected reply format
    const char * fmt = "%g\n";

    // Match within floating point tolerance
    float xy_actual;
    char msg[128];
    sprintf(msg, "Attempted to scan '%s' using format '%s'", out, fmt);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, sscanf(out, fmt, &xy_actual), msg);
    TEST_ASSERT_EQUAL_FLOAT(xy_expected, xy_actual);
}

TEST_GROUP_RUNNER(CmdXY)
{
    RUN_TEST_CASE(CmdXY, InitialXZero);
    RUN_TEST_CASE(CmdXY, InitialYZero);
    RUN_TEST_CASE(CmdXY, SetGetX);
    RUN_TEST_CASE(CmdXY, SetGetY);
    RUN_TEST_CASE(CmdXY, MultiplyXY);
}