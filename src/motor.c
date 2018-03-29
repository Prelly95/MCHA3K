#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <avr/io.h>
#include <util/twi.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "motor.h"

static volatile float Step_Mode; //Rad per step
static volatile float freq;
static volatile float Vel_Ref;
static volatile float yaw_left = 1;
static volatile float yaw_right = 1;
static volatile float on_a_dime = 0;

static volatile uint16_t isr_count_Left		= 0;
static volatile uint16_t isr_count_Right	= 0;
static volatile uint16_t isr_skips_Left		= 0;
static volatile uint16_t isr_skips_Right	= 0;
static volatile uint16_t OCR;

void Motor_Init(void)
{
	//set port A output pins
	DDRA |= _BV(DDA5) | _BV(DDA4) | _BV(DDA3) | _BV(DDA2) | _BV(DDA1) | _BV(DDA0);

	//set port B output pins
	DDRB |= _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2) | _BV(DDB1) | _BV(DDB0);

	DDRD |= (1<<PD6);

	Motor_Timer_Init();

	Motor_Step_Mode(QUARTER_STEP);
	Motor_Direction(FORWARD);
	Motor_Enable();
}

void Motor_Enable(void)
{
	PORTA &= ~(1 << EN_A);
	PORTB &= ~(1 << EN_B);
}

void Motor_Disable(void)
{
	PORTA |= _BV(EN_A);
	PORTB |= _BV(EN_B);
}

void Motor_Timer_Init(void)
{
	TCCR1B |= _BV(WGM12) | _BV(CS11);//CTC mode with no prescale

	OCR1AH = 0x00;
	OCR1AL = 0x23;
	OCR = (uint16_t)(OCR1AH<<8)|(uint16_t)(OCR1AL);
	uint8_t N = 8;
	freq = F_CPU/(2*N*(1 + OCR));

	TIMSK |= (1<<OCIE1A);
}

void Motor_Direction(MOTOR_DIRECTION DIR)
{
	switch(DIR)
	{
		case FORWARD:
		{
			PORTA &= ~(1 << DIR_A);
			PORTB |= _BV(DIR_B);
			break;
		}
		case BACKWARD:
		{
			PORTA |= _BV(DIR_A);
			PORTB &= ~(1 << DIR_B);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Motor_Step_Mode(STEP_MODE MODE)
{
	switch(MODE)
	{
		case FULL_STEP:
		{
			PORTA &= ~(1 << MODE0_A);
			PORTA &= ~(1 << MODE1_A);
			PORTA &= ~(1 << MODE2_A);

			PORTB &= ~(1 << MODE0_B);
			PORTB &= ~(1 << MODE1_B);
			PORTB &= ~(1 << MODE2_B);
			Step_Mode = 0.0314;
			break;
		}
		case HALF_STEP:
		{
			PORTA |= _BV(MODE0_A);
			PORTA &= ~(1 << MODE1_A);
			PORTA &= ~(1 << MODE2_A);

			PORTB |= _BV(MODE0_B);
			PORTB &= ~(1 << MODE1_B);
			PORTB &= ~(1 << MODE2_B);
			Step_Mode = 0.0157;
			break;
		}
		case QUARTER_STEP:
		{
			PORTA |= _BV(MODE0_A);
			PORTA |= _BV(MODE1_A);
			PORTA &= ~(1 << MODE2_A);

			PORTB |= _BV(MODE0_B);
			PORTB |= _BV(MODE1_B);
			PORTB &= ~(1 << MODE2_B);
			Step_Mode = 0.00785;
			break;
		}
		case EIGHT_STEP:
		{
			PORTA &= ~(1 << MODE0_A);
			PORTA &= ~(1 << MODE1_A);
			PORTA |= _BV(MODE2_A);

			PORTB &= ~(1 << MODE0_B);
			PORTB &= ~(1 << MODE1_B);
			PORTB |= _BV(MODE2_B);
			Step_Mode = 0.00393;
			break;
		}
		default:
		{
			break;
		}
	}
}

void Motor_Set_Skips(float dphi_left, float dphi_right)
{
	if(dphi_left>0.05)
	{
		Motor_Direction(FORWARD);
	}
	else if(dphi_left < -0.05)
	{
		dphi_left = -dphi_left;
		Motor_Direction(BACKWARD);
	}
	else
	{
		isr_skips_Left = 0xFF;
		return;
	}
	if(dphi_right>0.05)
	{
		Motor_Direction(FORWARD);
	}
	else if(dphi_right < -0.05)
	{
		dphi_right = -dphi_right;
		Motor_Direction(BACKWARD);
	}
	else
	{
		isr_skips_Right = 0xFF;
		return;
	}

	isr_skips_Left		= (Step_Mode/((dphi_left)/freq) - 1);
	isr_skips_Right		= (Step_Mode/((dphi_right)/freq) - 1);
}

void Motor_Set_Vref(float meters_p_s)
{

	Vel_Ref = meters_p_s;

}

float Motor_Get_Vref(void)
{
	return Vel_Ref;
}

void Motor_Set_Yaw(float yaw_left_in, float yaw_right_in, float on_a_dime_in)
{
	yaw_left = yaw_left_in;
	yaw_right = yaw_right_in;
	on_a_dime = on_a_dime_in;
}

void Motor_Get_Yaw(float *yaw_left_out, float *yaw_right_out, float *on_a_dime_out)
{
	*yaw_left_out = yaw_left;
	*yaw_right_out = yaw_right;
	*on_a_dime_out = on_a_dime;
}

void motor_step_isr(void)
{
	if(isr_count_Right >= isr_skips_Right)
	{
		PORTA |= _BV(STEP_A);
		isr_count_Right = 0;
	}
	else
	{
		PORTA &= ~_BV(STEP_A);
		++isr_count_Right;
	}

	if(isr_count_Left >= isr_skips_Left)
	{
		PORTB |= _BV(STEP_B);
		isr_count_Left = 0;
	}
	else
	{
		PORTB &= ~_BV(STEP_B);
		++isr_count_Left;
	}
}