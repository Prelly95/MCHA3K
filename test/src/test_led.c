#include "unity_fixture.h"

#include <stdint.h>
#include "avr/mock_sfr.h"
#include "led.h"

TEST_GROUP(LED);

TEST_SETUP(LED)
{
    led_init();
}

TEST_TEAR_DOWN(LED)
{
}

TEST(LED, PinConfig)
{
    mock_ddrb = 0x00; // All PortB set as input
    led_init(); // Expect only bit 3 set in least significant byte 
    TEST_ASSERT_BITS_HIGH_MESSAGE(0x08, mock_ddrb, "Expected DDRB.3 to be set");
    TEST_ASSERT_BITS_LOW_MESSAGE(0xF7, mock_ddrb, "Expected bits other than DDRB.3 remain unchanged");

    mock_ddrb = 0xFF; // All PortB set as output
    led_init(); // Expect all bits set in least significant byte
    TEST_ASSERT_BITS_MESSAGE(0xFF, 0xFF, mock_ddrb, "Expected bits other than DDRB.3 remain unchanged");
}

TEST(LED, DefaultBrightness)
{
    TEST_ASSERT_EQUAL_UINT8(0, led_get_brightness());
}

TEST(LED, TurnOn)
{
    led_on();
    TEST_ASSERT_EQUAL_UINT8(255, led_get_brightness());
}

TEST(LED, TurnOff)
{
    led_off();
    TEST_ASSERT_EQUAL_UINT8(0, led_get_brightness());
}

TEST(LED, SetBrightness)
{
    led_set_brightness(70);
    TEST_ASSERT_EQUAL_UINT8(70, led_get_brightness());
}

TEST_GROUP_RUNNER(LED)
{
    RUN_TEST_CASE(LED, PinConfig);
    RUN_TEST_CASE(LED, DefaultBrightness);
    RUN_TEST_CASE(LED, TurnOn);
    RUN_TEST_CASE(LED, TurnOff);
    RUN_TEST_CASE(LED, SetBrightness);
}