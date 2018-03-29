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
#include "oled.h"
// #include "fonts.h"

CLB_CREATE_STATIC(clb, 80);

int16_t x;
int16_t y;
int16_t z;

int main(void)
{
    uart_init();
	I2C_Init();
	OLED_Init();

    // Enable global interrupts
    sei();

    // Wait a second at startup
    _delay_ms(1000);

	// Send initial string
	printf_P(PSTR("\nHello world!\n\n"));

	OLED_SetCursor(20, 28);
	OLED_WriteString("HELLO WORLD");

	OLED_SetCursor(5, 40);
	OLED_WriteString("IMU ID 0X");

	OLED_PrintNum("%.1f", 1, 10.0);

	printf_P(PSTR("\nReady\n\n"));

    for(;/*ever*/;)
    {
        clb_process(&clb);
		// MPU6050_GetAcceleration(&x, &y, &z);
		// OLED_PrintNum(3, (float)x/1000, (float)y/1000, (float)z/1000);
		// OLED_PrintNum(3, 1.1, 1.2, 1.3);
    }
    return 0;
}
