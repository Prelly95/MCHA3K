#include <stdint.h>
#include <avr/io.h>
#include "potentiometer.h"

void pot_init(void)
{
    //Set AVCC to VREF
    ADCSRA |= (1 << ADEN);
    ADMUX |= (1 << REFS0);
}

uint16_t pot_get_value(void)
{
    //Start conversion
    ADCSRA |= (1 << ADSC);

    //wait until the conversion is done
    while(!(ADCSRA & (1 << ADIF)));

    ADCSRA |= (1 << ADIF);

    //Write the value to
    uint16_t val = ADCW;

    return val;
}
