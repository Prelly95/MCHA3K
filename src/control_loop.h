#ifndef LOOP_H
#define LOOP_H

void control_Init(void);
void control_enable(void);
void control_disable(void);
void control_loop_isr(void);
uint8_t control_triggered(void);
float control_run(float vref);
void _control_enable_trigger_isr(void);
void control_variables(float *theta_out,float *dtheta_out);
void Loop_Reset(void);
#endif