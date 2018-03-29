#include "unity_fixture.h"

#include <stdint.h>
#include "avr/io.h"
#include "avr/mock_sfr.h"
#include "potentiometer.h"

TEST_GROUP(Potentiometer);

TEST_SETUP(Potentiometer)
{
    pot_init();
    ADCSRA |= _BV(ADIF);        // Set conversion complete flag so any ADIF polling falls through
}

TEST_TEAR_DOWN(Potentiometer)
{
    ADCSRA &= ~(_BV(ADIF));     // Clear conversion complete flag after reading
    ADCW = 0;
}

TEST(Potentiometer, LittleEndianADCW)
{
    // Make sure the endianness of ADCW matches expected byte order
    // The GetValue test relies on this assumption
    ADCW = 0x1234;
    TEST_ASSERT_EQUAL_HEX8(0x12, ADCH);
    TEST_ASSERT_EQUAL_HEX8(0x34, ADCL);
}

TEST(Potentiometer, GetValueFromADC)
{
    uint16_t expected = 300; // Test value should be <= 1023

    if (bit_is_set(ADMUX, ADLAR))
        ADCW = expected << 6;
    else
        ADCW = expected;
    
    uint16_t actual = pot_get_value();

    TEST_ASSERT_EQUAL_UINT16(expected, actual);
}

TEST_GROUP_RUNNER(Potentiometer)
{
    RUN_TEST_CASE(Potentiometer, LittleEndianADCW);
    RUN_TEST_CASE(Potentiometer, GetValueFromADC);
}