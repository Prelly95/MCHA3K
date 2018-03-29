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
#include "mpu6050.h"
#include "motor.h"
#include "kalman.h"
#include "controller.h"
#include "control_loop.h"

CLB_CREATE_STATIC(clb, 80);

float theta_init = 0;
float dtheta_init = 0;
static volatile float dphi = 0;
static volatile int j = 0;

float yaw_left;
float yaw_right;
float on_a_dime;

int main(void)
{
	WDTCR |= (1 << WDTOE);
	WDTCR &= ~(1 << WDE);
	
    uart_init();
	I2C_Init();
	OLED_Init();
	MPU6050_Init();
	ctrl_init();
	Motor_Init();
	control_Init();

    // Enable global interrupts
    sei();

	_delay_ms(10);

	// Send initial string
	printf_P(PSTR("\014\nHello world!\n\n"));
	
	OLED_SetCursor(20, 0);
	OLED_WriteString("HELLO WORLD");

	_delay_ms(100);

	OLED_Fill(Black);
	OLED_UpdateScreen();

	while(j < 1)
	{
		control_variables(&theta_init, &dtheta_init);
		j++;
	}
	
	OLED_Fill(Black);
	OLED_UpdateScreen();
	OLED_SetCursor(45, 28);
	OLED_WriteString("ready");
	OLED_SetCursor(0, 0);

	control_enable();

    for(;/*ever*/;)
    {
		if(control_triggered())
		{
			dphi = control_run(Motor_Get_Vref());
		}
		
		Motor_Get_Yaw(&yaw_left, &yaw_right, &on_a_dime);

		Motor_Set_Skips(dphi*yaw_left+on_a_dime, dphi*yaw_right-on_a_dime);

		clb_process(&clb);
    }
    return 0;
}