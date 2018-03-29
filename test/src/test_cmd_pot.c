#include "unity_fixture.h"
#include "iospy.h"

#include <string.h>
#include "cmd_line_buffer.h"
#include "avr/io.h"
#include "avr/mock_sfr.h"
#include "potentiometer.h"

CLB_CREATE_STATIC(clb, 80);

TEST_GROUP(CmdPot);

TEST_SETUP(CmdPot)
{
    clb_init(&clb);
    pot_init();

    ADCSRA |= _BV(ADIF);        // Set conversion complete flag so any ADIF polling falls through
}

TEST_TEAR_DOWN(CmdPot)
{
    ADCSRA &= ~(_BV(ADIF));     // Clear conversion complete flag after reading
    ADCW = 0;
}

TEST(CmdPot, GetValue)
{
    uint16_t expected = 321; // Test value should be <= 1023

    if (bit_is_set(ADMUX, ADLAR))
        ADCW = expected << 6;
    else
        ADCW = expected;

    char out[80];

    iospy_hook();
    iospy_push_in_str("pot\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("Potentiometer ADC value is 321\n",out);
}

TEST(CmdPot, UnexpectedArgument)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("pot 420\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("pot: invalid argument \"420\", syntax is: pot\n", out);    
}

TEST_GROUP_RUNNER(CmdPot)
{
    RUN_TEST_CASE(CmdPot, GetValue);
    RUN_TEST_CASE(CmdPot, UnexpectedArgument);
}