#include <avr/io.h>

#ifndef MOTOR_H
#define MOTOR_H


// Pin Definitions
#define EN_A	PA0
#define MODE0_A PA1
#define MODE1_A PA2
#define MODE2_A PA3
#define DIR_A   PA4
#define STEP_A  PA5

#define EN_B	PB3  
#define MODE0_B PB2
#define MODE1_B PB1
#define MODE2_B PB0
#define DIR_B   PB4
#define STEP_B  PB5

typedef enum
{
    FULL_STEP,
    HALF_STEP,
    QUARTER_STEP,
    EIGHT_STEP
} STEP_MODE;

typedef enum
{
    FORWARD,
    BACKWARD
} MOTOR_DIRECTION;

void Motor_Timer_Init(void);
void Motor_Init(void);
void Motor_Direction(MOTOR_DIRECTION DIR);
void motor_step_isr(void);
void Motor_Step_Mode(STEP_MODE MODE);
void Motor_Enable(void);
void Motor_Disable(void);
void Motor_Set_Skips(float dphi_left, float dphi_right);
void Motor_Set_Vref(float meters_p_s);
float Motor_Get_Vref(void);
void Motor_Set_Yaw(float yaw_left_in, float yaw_right_in, float on_a_dime_in);
void Motor_Get_Yaw(float *yaw_left_out, float *yaw_right_out, float *on_a_dime_out);

#endif