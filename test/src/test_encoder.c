#include "unity_fixture.h"

#include <stdint.h>
#include "avr/io.h"
#include "avr/mock_sfr.h"
#include "encoder.h"

TEST_GROUP(Encoder);

TEST_SETUP(Encoder)
{
    encoder_init();
}

TEST_TEAR_DOWN(Encoder)
{
}

TEST(Encoder, DefaultCount)
{
    TEST_ASSERT_EQUAL_INT32(0, encoder_get_count());
}

TEST(Encoder, SetCount)
{
    encoder_set_count(42);
    TEST_ASSERT_EQUAL_INT32(42, encoder_get_count());
}

static void raise_A(void) { mock_portd |=   1 << 2 ; }  // Raise PD2/INT0/chA
static void raise_B(void) { mock_portd |=   1 << 3 ; }  // Raise PD3/INT1/chB
static void lower_A(void) { mock_portd &= ~(1 << 2); }  // Lower PD2/INT0/chA
static void lower_B(void) { mock_portd &= ~(1 << 3); }  // Lower PD3/INT1/chB

TEST(Encoder, QuadratureIncrement)
{
    PIND = 0x00;
    encoder_set_count(0);

    // Transition from state 1 to 2
    raise_B();
    encoder_edge_B_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, encoder_get_count(), "Expected increment for state 1 to 2 transition");

    // Transition from state 2 to 3
    raise_A();
    encoder_edge_A_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, encoder_get_count(), "Expected increment for state 2 to 3 transition");

    // Transition from state 3 to 4
    lower_B();
    encoder_edge_B_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, encoder_get_count(), "Expected increment for state 3 to 4 transition");

    // Transition from state 4 to 1
    lower_A();
    encoder_edge_A_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, encoder_get_count(), "Expected increment for state 4 to 1 transition");

    // Transition from state 1 to 2
    raise_B();
    encoder_edge_B_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, encoder_get_count(), "Expected increment for state 1 to 2 transition");
}

TEST(Encoder, QuadratureDecrement)
{
    mock_portd = 0x00;
    encoder_set_count(0);

    // Transition from state 1 to 4
    raise_A();
    encoder_edge_A_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, encoder_get_count(), "Expected decrement for state 1 to 4 transition");

    // Transition from state 4 to 3
    raise_B();
    encoder_edge_B_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-2, encoder_get_count(), "Expected decrement for state 4 to 3 transition");

    // Transition from state 3 to 2
    lower_A();
    encoder_edge_A_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-3, encoder_get_count(), "Expected decrement for state 3 to 2 transition");

    // Transition from state 2 to 1
    lower_B();
    encoder_edge_B_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-4, encoder_get_count(), "Expected decrement for state 2 to 1 transition");

    // Transition from state 1 to 4
    raise_A();
    encoder_edge_A_isr();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-5, encoder_get_count(), "Expected decrement for state 1 to 4 transition");
}

TEST(Encoder, PopCount)
{
    encoder_set_count(123);
    TEST_ASSERT_EQUAL_INT32(123, encoder_pop_count());
    TEST_ASSERT_EQUAL_INT32(0, encoder_get_count());
}

TEST_GROUP_RUNNER(Encoder)
{
    RUN_TEST_CASE(Encoder, DefaultCount);
    RUN_TEST_CASE(Encoder, SetCount);
    RUN_TEST_CASE(Encoder, QuadratureIncrement);
    RUN_TEST_CASE(Encoder, QuadratureDecrement);
    RUN_TEST_CASE(Encoder, PopCount);
}