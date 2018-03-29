#include <stdint.h>
// #include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "led.h"
#include "encoder.h"
#include "potentiometer.h"
#include "dimmer.h"
#include "uart.h"
#include "cmd_line_buffer.h"
#include "cmd_parser.h"

CLB_CREATE_STATIC(clb, 80);

int main(void)
{
    // Initialise modules
    led_init();
    encoder_init();
    pot_init();
    uart_init();

    // Enable global interrupts
    sei();

    // Wait a second at startup
    // _delay_ms(1000);
    _delay_ms(200); led_on();
    _delay_ms(200); led_off();
    _delay_ms(200); led_on();
    _delay_ms(200); led_off();
    _delay_ms(200); led_on();

    // Send initial string
    printf_P(PSTR("\nHello world!\n\n"));

    for(;/*ever*/;)
    {
        // Do some pretend processing (do not remove delays)
        // _delay_ms(125); led_on();
        // _delay_ms(125); led_off();

        clb_process(&clb);
    }
    return 0;
}
