#include <avr/interrupt.h>
#include "uart.h"

ISR(USART_RXC_vect)         // USART RX interrupt
{
    uart_rxc_isr();
}

ISR(USART_UDRE_vect)        // USART UDRE interrupt
{
    uart_udre_isr();
}