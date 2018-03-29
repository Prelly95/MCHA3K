#include "unity_fixture.h"

#include <stdint.h>
#include "task.h"

TEST_GROUP(Task);

TEST_SETUP(Task)
{
    task_init();
}

TEST_TEAR_DOWN(Task)
{
}

TEST(Task, InitNullCallback)
{
    TEST_ASSERT_NULL(task_get());
}

TEST(Task, InitDisabled)
{
    TEST_ASSERT_FALSE(task_is_enabled());
}

TEST(Task, Enable)
{
    task_disable();
    task_enable();
    TEST_ASSERT_TRUE(task_is_enabled());
}

TEST(Task, Disable)
{
    task_enable();
    task_disable();
    TEST_ASSERT_FALSE(task_is_enabled());
}

static void _dummy(void) { }

TEST(Task, SetGetCallback)
{
    task_set(_dummy);
    TEST_ASSERT_EQUAL_PTR(_dummy, task_get());
}

static uint32_t _call_count;
static void _stub(void) { ++_call_count; }

TEST(Task, RunCallback)
{
    _call_count = 0;
    task_set(_stub);
    task_run();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, _call_count, "Expected event callback to be called once.");
}

TEST(Task, TriggerIncr)
{
    task_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(1, task_triggered());
    task_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(2, task_triggered());
}

TEST(Task, RunClearsTrigger)
{
    task_set(_stub);
    task_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(1, task_triggered());
    task_run();
    TEST_ASSERT_EQUAL_UINT8(0, task_triggered());
}

#include "iospy.h"

TEST(Task, RunNull)
{
    task_set(NULL);
    task_enable();
    iospy_hook_out();
    task_run();
    iospy_unhook_out();
    TEST_ASSERT_FALSE(task_is_enabled());
}

static void _stub2(void) { task_trigger_isr(); }

TEST(Task, DisableAfterTimeout)
{
    task_set(_stub2);
    iospy_hook_out();
    task_run();
    iospy_unhook_out();
    TEST_ASSERT_FALSE(task_is_enabled());
}

TEST_GROUP_RUNNER(Task)
{
    RUN_TEST_CASE(Task, InitNullCallback);
    RUN_TEST_CASE(Task, InitDisabled);
    RUN_TEST_CASE(Task, Enable);
    RUN_TEST_CASE(Task, Disable);
    RUN_TEST_CASE(Task, SetGetCallback);
    RUN_TEST_CASE(Task, RunCallback);
    RUN_TEST_CASE(Task, TriggerIncr);
    RUN_TEST_CASE(Task, RunClearsTrigger);
    RUN_TEST_CASE(Task, RunNull);
    RUN_TEST_CASE(Task, DisableAfterTimeout);
}