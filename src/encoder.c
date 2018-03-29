#include <stdint.h>
#include <avr/io.h>
#include "encoder.h"

static int32_t _count = 0;

// Forward decl of private helper functions
static void _encoder_enable_interrupts(void);
static void _encoder_disable_interrupts(void);

void encoder_init(void)
{
    //Enable pullup resistors (if not done in hardware)
    PORTD |= (1 << PD2)|(1 << PD3);

    //Configure interrupt edge triggers for INT0 and INT1

    //Enable INT1 Change trigger mode
    MCUCR |= (1 << ISC00);
    //Enable INT0 Rising edge
    MCUCR |= (1 << ISC10);
    // MCUCR |= (1 << ISC11);

    // Set default count
    _count = 0;

    // Enable interrupts INT0 and INT1
    _encoder_enable_interrupts();
}

void encoder_edge_A_isr(void)
{
    uint8_t A = !!(PIND & (1 << PD2));
    uint8_t B = !!(PIND & (1 << PD3));
    if(A == B)
    {
        _count++;
    }
    else
    {
        _count--;
    }
}

void encoder_edge_B_isr(void)
{

    uint8_t A = !!(PIND & (1 << PD2));
    uint8_t B = !!(PIND & (1 << PD3));

    if(A == B)
    {
        _count--;
    }
    else
    {
        _count++;
    }
}

void encoder_set_count(int32_t count)
{
    // Atomically set _count
    _encoder_disable_interrupts();
    _count = count;
    _encoder_enable_interrupts();
}

int32_t encoder_get_count(void)
{
    // Atomically read _count
    _encoder_disable_interrupts();
    uint32_t count = _count;
    _encoder_enable_interrupts();
    return count;
}

int32_t encoder_pop_count(void)
{
    // Atomically read and reset _count
    _encoder_disable_interrupts();
    int32_t count = _count;
    _count = 0;
    _encoder_enable_interrupts();
    return count;
}

void _encoder_enable_interrupts(void)
{
    //Enable INT1 Change trigger mode
    GICR |= (1 << INT0);
    //Enable INT0 Rising edge
    GICR |= (3 << INT1);
}

void _encoder_disable_interrupts(void)
{
  //Disable INT1 Change trigger mode
  GICR &= (0 << INT0);
  //Disable INT0 Rising edge
  GICR &= (0 << ISC01)&(0 << INT1);
}
