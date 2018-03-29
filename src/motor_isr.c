#include <avr/interrupt.h>
#include "motor.h"

ISR(TIMER1_COMPA_vect)
{
    motor_step_isr();
}