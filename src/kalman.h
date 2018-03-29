#ifndef _KALMAN_H_
#define _KALMAN_H_

typedef struct
{
    float Pww;
    float Pwt;
    float Pwb;
    float Ptt;
    float Ptb;
    float Pbb;

    float uw;
    float ut;
    float ub;
} kVariables_s;

uint8_t Kalman_Init(uint8_t pw, uint8_t pb, float T);
void Kalman_TimeUpdate(uint16_t qw, float qb, float T);

float Kalman_Accelerometer(double theta, uint8_t rt);
float Kalman_Gyro(double dtheta, float rw);
void Kalman_Process(float* theta_Out, float* dtheta_Out, float theta_In, float dtheta_In);

#endif