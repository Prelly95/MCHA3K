#include "unity_fixture.h"

#include <stdint.h>
#include "controller.h"

TEST_GROUP(Ctrl);

TEST_SETUP(Ctrl)
{
    ctrl_init();
}

TEST_TEAR_DOWN(Ctrl)
{
}

TEST(Ctrl, ZeroInitialState)
{
    TEST_ASSERT_EACH_EQUAL_FLOAT(0.0f, ctrl_get_state(), CTRL_N_STATE);
}

TEST(Ctrl, SetState)
{
    float state_expected[CTRL_N_STATE];
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        state_expected[i] = 1.2f*i;

    ctrl_set_state(state_expected);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(state_expected, ctrl_get_state(), CTRL_N_STATE);
}

TEST(Ctrl, OneStepAx)
{
    float x0[CTRL_N_STATE] = {0.1};
    float u[CTRL_N_INPUT] = {0, 0, 0};

    ctrl_set_state(x0);
    ctrl_run(u);
    float * x_actual = ctrl_get_state();

    float x_expected[CTRL_N_STATE] = {0.077548};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x_expected, x_actual, CTRL_N_STATE);
}

TEST(Ctrl, OneStepBu)
{
    float x0[CTRL_N_STATE] = {0};
    float u[CTRL_N_INPUT] = {1, -2, 0};

    ctrl_set_state(x0);
    ctrl_run(u);
    float * x_actual = ctrl_get_state();

    float x_expected[CTRL_N_STATE] = {0.068152};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x_expected, x_actual, CTRL_N_STATE);
}

TEST(Ctrl, OutputCx)
{
    float x0[CTRL_N_STATE] = {0.1};
    float u[CTRL_N_INPUT] = {0, 0, 0};

    ctrl_set_state(x0);

    float y_expected[CTRL_N_OUTPUT] = {3.719};
    float * y_actual = ctrl_run(u);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(y_expected, y_actual, CTRL_N_OUTPUT);
}

TEST(Ctrl, OutputDu)
{
    float x0[CTRL_N_STATE] = {0};
    float u[CTRL_N_INPUT] = {1, -2, 0};

    ctrl_set_state(x0);

    float y_expected[CTRL_N_OUTPUT] = {-14.352};
    float * y_actual = ctrl_run(u);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(y_expected, y_actual, CTRL_N_OUTPUT);
}

TEST(Ctrl, RunSteps)
{
    float x0[CTRL_N_STATE] = {0.1};

    float v_ref[] = {1, 1.001, 1.002};
    float v[] = {-2.000000, -1.998000, -1.996000};
    float theta[] = {0, 0.0001, 0.0002};

    enum {N_DATA = sizeof(v_ref)/sizeof(v_ref[0])};

    float y_expected[][CTRL_N_OUTPUT] = {{-10.633}, {-8.917}, {-7.5853}};

    float u[CTRL_N_INPUT];

    ctrl_set_state(x0);

    for (size_t i = 0; i < N_DATA; ++i)
    {
        u[0] = v_ref[i];
        u[1] = v[i];
        u[2] = theta[i];
        float * y_actual = ctrl_run(u);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(&y_expected[i][0], y_actual, CTRL_N_OUTPUT);
    }
}

TEST_GROUP_RUNNER(Ctrl)
{
    RUN_TEST_CASE(Ctrl, ZeroInitialState);
    RUN_TEST_CASE(Ctrl, SetState);
    RUN_TEST_CASE(Ctrl, OneStepAx);
    RUN_TEST_CASE(Ctrl, OneStepBu);
    RUN_TEST_CASE(Ctrl, OutputCx);
    RUN_TEST_CASE(Ctrl, OutputDu);
    RUN_TEST_CASE(Ctrl, RunSteps);
}
