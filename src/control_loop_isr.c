#include <avr/interrupt.h>
#include "control_loop.h"
#include "motor.h"

ISR(TIMER2_COMP_vect)
{
    control_loop_isr();
}