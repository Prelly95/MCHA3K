#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <math.h>
#include <util/delay.h>

#include "mpu6050.h"
#include "control_loop.h"
#include "motor.h"
#include "kalman.h"
#include "controller.h"
#include "cmd_parser.h"

static volatile uint8_t _control_trigger_count;


uint8_t control_triggered(void)
{
    return _control_trigger_count;
}

void control_Init(void)
{
	TCCR2 |= (1<<WGM21);						//CTC mode
	TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20); 	//prescale clk_T2s/1024, 
	TCCR2 |= (1<<COM20);						//Toggle OC2 on compare match										

	OCR2 = 143;

	DDRD|=(1<<PD7);
	DDRD|=(1<<PD6);

	control_disable();
}

void control_enable(void)
{
    _control_trigger_count = 0;
	
	TCNT2 = 0;
	_control_enable_trigger_isr();
	TCCR2 |= (1<<FOC2);  
}

void control_disable(void)
{
	_control_trigger_count = 0;
    TIMSK &= ~(1 << OCIE2);
}

void _control_enable_trigger_isr(void)
{
    TIMSK |= (1<<OCIE2);
}

float control_run(float vref)
{
	PORTD |= (1 << PD6);
	float theta;
	float dtheta;
	float ctrl_in[3];
	float control_action;

	control_variables(&theta, &dtheta);

	ctrl_in[0] = theta;
	ctrl_in[1] = dtheta;
	ctrl_in[2] = vref;

	control_action = *ctrl_run(ctrl_in);
	
	if(theta > 0.8 || theta < -0.8)
	{
		Loop_Reset();
	}
	PORTD &= !(1 << PD6);
	_control_trigger_count = 0;

	return control_action;
}

void control_variables(float *theta_out, float *dtheta_out)
{
	float imu_dtheta;
	float imu_theta;

	float dtheta_t;
	float theta_t;

	MPU6050_Balance(&imu_dtheta, &imu_theta);
	Kalman_Process(&theta_t, &dtheta_t, imu_theta, imu_dtheta);

	*theta_out = theta_t;
	*dtheta_out = dtheta_t;
}

void control_loop_isr(void)
{
	++_control_trigger_count;
}

void Loop_Reset(void)
{
	WDTCR &= ~(1 << WDP2);
	WDTCR &= ~(1 << WDP1);
	WDTCR &= ~(1 << WDP0);
	WDTCR |= (1 << WDE);

	while(1);
}