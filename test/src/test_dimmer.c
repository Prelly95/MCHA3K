#include "unity_fixture.h"
#include "dimmer.h"

TEST_GROUP(Dimmer);

TEST_SETUP(Dimmer)
{
    dimmer_init();
}

TEST_TEAR_DOWN(Dimmer)
{
}

TEST(Dimmer, DefaultBrightness)
{
    TEST_ASSERT_EQUAL_UINT8(DIMMER_DEFAULT_BRIGHTNESS, dimmer_get_brightness());
}

TEST(Dimmer, DefaultStep)
{
    TEST_ASSERT_EQUAL_UINT8(DIMMER_DEFAULT_STEP, dimmer_get_step());
}

TEST(Dimmer, SetBrightness)
{
    dimmer_set_brightness(42);
    TEST_ASSERT_EQUAL_UINT8(42, dimmer_get_brightness());
}

TEST(Dimmer, MinimumBrightness)
{
    dimmer_set_brightness(0);
    TEST_ASSERT_EQUAL_UINT8(DIMMER_MIN_BRIGHTNESS, dimmer_get_brightness());
}

TEST(Dimmer, MaximumBrightness)
{
    dimmer_set_brightness(255);
    TEST_ASSERT_EQUAL_UINT8(DIMMER_MAX_BRIGHTNESS, dimmer_get_brightness());
}


TEST(Dimmer, SetStep)
{
    dimmer_set_step(17);
    TEST_ASSERT_EQUAL_UINT8(17, dimmer_get_step());
}

TEST(Dimmer, MinimumStep)
{
    dimmer_set_step(0);
    TEST_ASSERT_EQUAL_UINT8(DIMMER_MIN_STEP, dimmer_get_step());
}

TEST(Dimmer, MaximumStep)
{
    dimmer_set_step(255);
    TEST_ASSERT_EQUAL_UINT8(DIMMER_MAX_STEP, dimmer_get_step());
}

TEST(Dimmer, IncrementByStep)
{
    dimmer_set_brightness(5);
    dimmer_set_step(10);

    dimmer_increment();
    TEST_ASSERT_EQUAL_UINT8(15, dimmer_get_brightness());
}

TEST(Dimmer, DecrementByStep)
{
    dimmer_set_brightness(25);
    dimmer_set_step(10);

    dimmer_decrement();
    TEST_ASSERT_EQUAL_UINT8(15, dimmer_get_brightness());
}

TEST(Dimmer, Change)
{
    dimmer_set_brightness(35);

    dimmer_set_step(2);
    dimmer_change(5);
    TEST_ASSERT_EQUAL_UINT8(45, dimmer_get_brightness());

    dimmer_set_step(1);
    dimmer_change(-10);
    TEST_ASSERT_EQUAL_UINT8(35, dimmer_get_brightness());
}

TEST(Dimmer, IncrementOverflow)
{
    dimmer_set_brightness(DIMMER_MAX_BRIGHTNESS - 10);
    dimmer_set_step(20);

    dimmer_increment();
    TEST_ASSERT_EQUAL_UINT8(DIMMER_MAX_BRIGHTNESS, dimmer_get_brightness());
}

TEST(Dimmer, DecrementOverflow)
{
    dimmer_set_brightness(DIMMER_MIN_BRIGHTNESS + 10);
    dimmer_set_step(20);

    dimmer_decrement();
    TEST_ASSERT_EQUAL_UINT8(DIMMER_MIN_BRIGHTNESS, dimmer_get_brightness());
}

TEST_GROUP_RUNNER(Dimmer)
{
    RUN_TEST_CASE(Dimmer, DefaultBrightness);
    RUN_TEST_CASE(Dimmer, DefaultStep);
    RUN_TEST_CASE(Dimmer, SetBrightness);
    RUN_TEST_CASE(Dimmer, MinimumBrightness);
    RUN_TEST_CASE(Dimmer, MaximumBrightness);
    RUN_TEST_CASE(Dimmer, SetStep);
    RUN_TEST_CASE(Dimmer, MinimumStep);
    RUN_TEST_CASE(Dimmer, MaximumStep);
    RUN_TEST_CASE(Dimmer, IncrementByStep);
    RUN_TEST_CASE(Dimmer, DecrementByStep);
    RUN_TEST_CASE(Dimmer, Change);
    RUN_TEST_CASE(Dimmer, IncrementOverflow);
    RUN_TEST_CASE(Dimmer, DecrementOverflow);
}