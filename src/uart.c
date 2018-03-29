#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "uart.h"
#include "circ_buffer.h"

CB_CREATE_STATIC(rx_buf, 64);
CB_CREATE_STATIC(tx_buf, 256); //Reminder: change this to 256 later

// Declare stream functions
static int _uart_putchar(char c, FILE *);
static int _uart_getchar(FILE *);
// Declare and initialise stream used for stdio functions
static FILE _uart_str = FDEV_SETUP_STREAM(_uart_putchar, _uart_getchar, _FDEV_SETUP_RW);

int _uart_receive(void);

void uart_init(void)
{
	//Initialise software FIFO Buffers
	cb_init(&rx_buf);
	cb_init(&rx_buf);

    // Set baud rate
    UBRRH = (uint8_t)(UART_CALC_BAUDRATE(UART_BAUD_RATE) >> 8);
    UBRRL = (uint8_t)(UART_CALC_BAUDRATE(UART_BAUD_RATE));

	#ifdef UART_DOUBLESPEED
	    UCSRA = _BV(U2X);
	#endif

	// Enable receiver and transmitter
	UCSRB = _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);

	//Enable RXC and UDR interrupts
	UCSRB |= _BV(RXCIE) | _BV(UDRIE);

    // Asynchronous 8N1
    UCSRC = _BV(URSEL) | (3 << UCSZ0);

    // Initialise standard IO streams
    stdout = stdin = &_uart_str;

}

int _uart_getchar(__attribute__((unused)) FILE *stream)
{
  if(cb_is_empty(&rx_buf))
  {
    return EOF;		//let caller know that no character is waiting
  }
  else
  {
    return cb_dequeue(&rx_buf);
  }

}

int _uart_putchar(char c, __attribute__((unused)) FILE *stream)
{
    while(cb_is_full(&tx_buf)){/*wait*/};			// wait until we have room in the TX buffer
    cb_queue(&tx_buf, c);				// buffer char
	UCSRB |= _BV(UDRIE);				//enable UDRIE Interrupt USART Data Register Empty Interrupt

    return 0;							//success
}

int _uart_receive(void)
{
    uint8_t status = UCSRA;         // Need to read UCSRA before reading UDR
    uint8_t c = UDR;                // Read UDR to clear RXC flag and swap hardware FIFO buffers

    if (status & (_BV(FE) | _BV(DOR) | _BV(PE)))
    {
        if (bit_is_set(status, FE))  printf_P(PSTR("*** UART framing error! ***\n"));
        if (bit_is_set(status, DOR)) printf_P(PSTR("*** UART data overrun! ***\n"));
        if (bit_is_set(status, PE))  printf_P(PSTR("*** UART parity error! ***\n"));
        c = EOF;
    }

    return c;
}

void uart_rxc_isr(void)
{
	int c;
	if((c = _uart_receive()) != EOF)
	{
		cb_queue(&rx_buf, c);
	}
}

void uart_udre_isr(void)
{
    if(cb_is_empty(&tx_buf))
	{
		UCSRB &= ~(_BV(UDRIE));
	}
	else
	{
		UDR = cb_dequeue(&tx_buf);
	}
}
