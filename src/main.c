#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <util/delay.h>
#include "uart.h"
#include "cmd_line_buffer.h"
#include "cmd_parser.h"

CLB_CREATE_STATIC(clb, 80);

int main(void)
{
    uart_init();
    
    // Enable global interrupts
    sei();      

    // Send initial string
    printf_P(PSTR("\nHello world!\n\n"));

    for(;/*ever*/;)
    {
        clb_process(&clb);
    }
    return 0;
}