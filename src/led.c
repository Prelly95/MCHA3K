#include <stdint.h>
#include <avr/io.h>
#include "led.h"

void timer0_init(void)
{
    //set timer to fast PWM Mode, prescaler to 1 and Clear OC0 on Compare Match
    TCCR0 |= (1 << WGM00)|(1 << CS00)|(1 << COM01)|(1 << COM00);
    OCR0 = 0;
}

void led_init(void)
{
    DDRB |= _BV(PB3);
    timer0_init();
}

void led_on(void)
{
    OCR0 = 255;
}

void led_off(void)
{
    OCR0 = 0;
}

void led_set_brightness(uint8_t value)
{
    OCR0 = value;
}

uint8_t led_get_brightness(void)
{
    return OCR0;
}
