#include "unity_fixture.h"
#include "stdio_redirect.h"

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
    char out[80];
    uint16_t expected = 321; // Test value should be <= 1023

    if (bit_is_set(ADMUX, ADLAR))
        ADCW = expected << 6;
    else
        ADCW = expected;

    push_stdio();
    fputs("pot\n",fstdin());
    rewind(fstdin());
    clb_process(&clb);
    if (feof(stdin)) clearerr(stdin); // <-- Secret sauce

    rewind(stdout);
    fgets(out, sizeof(out), fstdout());
    pop_stdio();

    TEST_ASSERT_EQUAL_STRING("Potentiometer ADC value is 321\n",out);
}

TEST(CmdPot, UnexpectedArgument)
{
    char out[80];

    push_stdio();
    fputs("pot 420\n",fstdin());
    rewind(fstdin());
    clb_process(&clb);
    if (feof(stdin)) clearerr(stdin); // <-- Secret sauce

    rewind(stdout);
    fgets(out, sizeof(out), fstdout());
    pop_stdio();

    TEST_ASSERT_EQUAL_STRING("pot: invalid argument \"420\", syntax is: pot\n", out);    
}

TEST_GROUP_RUNNER(CmdPot)
{
    RUN_TEST_CASE(CmdPot, GetValue);
    RUN_TEST_CASE(CmdPot, UnexpectedArgument);
}