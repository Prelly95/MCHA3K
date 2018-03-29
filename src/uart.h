#ifndef UART_H
#define UART_H

#define UART_BAUD_RATE 115200

//#define UART_DOUBLESPEED

#ifdef UART_DOUBLESPEED
#define UART_CALC_BAUDRATE(baud) ((uint32_t)((F_CPU) + ((uint32_t)baud * 4UL)) / ((uint32_t)(baud) * 8UL) - 1)
#else
#define UART_CALC_BAUDRATE(baud) ((uint32_t)((F_CPU) + ((uint32_t)baud * 8UL)) / ((uint32_t)(baud) * 16UL) - 1)
#endif

void uart_init(void);

void uart_rxc_isr(void);
void uart_udre_isr(void);

#endif
