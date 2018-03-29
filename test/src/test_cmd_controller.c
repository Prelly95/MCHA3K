#include "unity_fixture.h"
#include "iospy.h"
#include <stdint.h>
#include "cmd_line_buffer.h"
#include "controller.h"

static void _given(const char *, float);

CLB_CREATE_STATIC(clb, 80);

TEST_GROUP(CmdCtrl);

TEST_SETUP(CmdCtrl)
{
    clb_init(&clb);
    ctrl_init();
}

TEST_TEAR_DOWN(CmdCtrl)
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

TEST(CmdCtrl, SetVRefSilent)
{
    _given("vref", 3.14f);
}

TEST(CmdCtrl, SetVSilent)
{
    _given("v", 2.72f);
}

TEST(CmdCtrl, SetThetaSilent)
{
    _given("theta", 1.62f);
}

TEST(CmdCtrl, CtrlReturnsValue)
{
    char out[1024];

    iospy_hook();
    iospy_push_in_str("ctrl\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    // Expected reply format
    const char * fmt = "%g\n";

    float y_actual;
    char msg[1024];
    sprintf(msg, "Attempted to scan '%s' using format '%s'", out, fmt);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, sscanf(out, fmt, &y_actual), msg);
}

IGNORE_TEST(CmdCtrl, OneStep)
{
    float x0[CTRL_N_STATE] = {0.1, 0.1};
    ctrl_set_state(x0);

    float u[CTRL_N_INPUT] = {1, -2, 0};
    _given("vref", u[0]);
    _given("v", u[1]);
    _given("theta", u[2]);

    char out[1024];

    iospy_hook();
    iospy_push_in_str("ctrl\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    // Expected reply format
    const char * fmt = "%g\n";

    float y_expected = 0.15;
    float y_actual;
    char msg[1024];
    sprintf(msg, "Attempted to scan '%s' using format '%s'", out, fmt);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, sscanf(out, fmt, &y_actual), msg);
    TEST_ASSERT_EQUAL_FLOAT(y_expected, y_actual);
}

TEST_GROUP_RUNNER(CmdCtrl)
{
    RUN_TEST_CASE(CmdCtrl, SetVRefSilent);
    RUN_TEST_CASE(CmdCtrl, SetVSilent);
    RUN_TEST_CASE(CmdCtrl, SetThetaSilent);
    RUN_TEST_CASE(CmdCtrl, CtrlReturnsValue);
    RUN_TEST_CASE(CmdCtrl, OneStep);
}