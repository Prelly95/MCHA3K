#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/twi.h>
#include <util/atomic.h>
#include "uart.h"
#include "cmd_line_buffer.h"
#include "cmd_parser.h"
#include "i2c.h"
#include "mpu6050.h"

CLB_CREATE_STATIC(clb, 80);

int16_t x;
int16_t y;
int16_t z;

int main(void)
{
    uart_init();
	I2C_Init();
	MPU6050_Init();

    // Enable global interrupts
    sei();

    // Wait a second at startup
    _delay_ms(1000);

	// Send initial string
	printf_P(PSTR("\nHello world!\n\n"));

	printf_P(PSTR("\nReady\n\n"));

    for(;/*ever*/;)
    {
        clb_process(&clb);
		MPU6050_GetRotation(&x, &y, &z);
		printf_P(PSTR("X: %i\t"), x);
		printf_P(PSTR("Y: %i\t"), y);
		printf_P(PSTR("Z: %i\n"), z);
    }
    return 0;
}
