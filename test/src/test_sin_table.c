#include "unity_fixture.h"
#include "iospy.h"

#include "task.h"
#include "sin_table.h"

TEST_GROUP(SinTable);

TEST_SETUP(SinTable)
{
    task_init();
}

TEST_TEAR_DOWN(SinTable)
{
    task_init(); // Undo side effects of sin_table
}

TEST(SinTable, TaskEnabled)
{
    char out[80];

    int argc = 4;
    const char *argv[] = {"sin", "1", "0", "0", NULL};

    iospy_hook_out();
    sin_table_cmd(argc, argv);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook_out();

    TEST_ASSERT_EQUAL_PTR(sin_table_task, task_get());
    TEST_ASSERT_TRUE(task_is_enabled());
}

TEST(SinTable, TaskDisabled)
{
    char out[80];

    int argc = 4;
    const char *argv[] = {"sin", "1", "0", "0", NULL};

    iospy_hook_out();
    sin_table_cmd(argc, argv);
    sin_table_task();
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook_out();

    TEST_ASSERT_FALSE(task_is_enabled());
}

TEST(SinTable, Header)
{
    char out[80];

    int argc = 4;
    const char *argv[] = {"sin", "0", "0", "0", NULL};

    iospy_hook_out();
    sin_table_cmd(argc, argv);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook_out();

    TEST_ASSERT_EQUAL_STRING("Time [sec],Value\n", out);
}

TEST(SinTable, DataFormat)
{
    char out[80];

    int argc = 4;
    const char *argv[] = {"sin", "1", "0", "0", NULL};

    iospy_hook();
    sin_table_cmd(argc, argv);
    iospy_pop_out_str(out, sizeof(out)); // Ignore header
    sin_table_task();
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    // Expected reply format
    const char * fmt = "%g,%g\n";

    float t_actual, v_actual;
    char msg[1024];
    sprintf(msg, "Attempted to scan '%s' using format '%s'", out, fmt);

    TEST_ASSERT_EQUAL_INT_MESSAGE(2, sscanf(out, fmt, &t_actual, &v_actual), msg);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, t_actual);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, v_actual);
}

TEST(SinTable, Example)
{
    float t_expected[] = {0.00f, 0.01f, 0.02f, 0.03f, 0.04f};
    float v_expected[] = {0.0f, 0.425779f, 0.770513f, 0.968583f, 0.982287f};

    enum {N_DATA = sizeof(t_expected)/sizeof(t_expected[0])};

    char out[5][80];

    int argc = 4;
    const char *argv[] = {"sin", "5", "1", "7", NULL};

    iospy_hook();
    sin_table_cmd(argc, argv);
    iospy_pop_out_str(&out[0][0], 80); // Ignore header
    for (size_t i = 0; i < N_DATA; ++i)
    {
        sin_table_task();
        iospy_pop_out_str(&out[i][0], 80);
    }
    iospy_unhook();

    // Expected reply format
    const char * fmt = "%g,%g\n";

    float t_actual, v_actual;
    char msg[1024];

    for (size_t i = 0; i < N_DATA; ++i)
    {
        sprintf(msg, "Attempted to scan '%s' using format '%s'", &out[i][0], fmt);

        TEST_ASSERT_EQUAL_INT_MESSAGE(2, sscanf(&out[i][0], fmt, &t_actual, &v_actual), msg);

        TEST_ASSERT_EQUAL_FLOAT(t_expected[i], t_actual);
        TEST_ASSERT_EQUAL_FLOAT(v_expected[i], v_actual);
    }
}

TEST_GROUP_RUNNER(SinTable)
{
    RUN_TEST_CASE(SinTable, TaskEnabled);
    RUN_TEST_CASE(SinTable, TaskDisabled);
    RUN_TEST_CASE(SinTable, Header);
    RUN_TEST_CASE(SinTable, DataFormat);
    RUN_TEST_CASE(SinTable, Example);
}