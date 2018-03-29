#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/twi.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "kalman.h"

kVariables_s Kalman_Variables;
uint8_t KalmanIsInit = 0;

uint8_t Kalman_Init(uint8_t pw, uint8_t pb, float T)
{
    Kalman_Variables.Pww = T*(float)pw;
    Kalman_Variables.Pwt = (0.5)*T*T*(float)pw;
    Kalman_Variables.Pwb = 0;
    
    Kalman_Variables.Ptt = (0.3333)*T*T*T*(float)pw;
    Kalman_Variables.Ptb = 0;
    
    Kalman_Variables.Pbb = T*(float)pb;
    
    Kalman_Variables.uw = 0;
    Kalman_Variables.ut = 0;
    Kalman_Variables.ub = 0;

    return 1;
}

void Kalman_TimeUpdate(uint16_t qw, float qb, float T)
{
    float Pww_t = Kalman_Variables.Pww;
    float Pwt_t = Kalman_Variables.Pwt;
    float Pwb_t = Kalman_Variables.Pwb;
    float Ptt_t = Kalman_Variables.Ptt;
    float Ptb_t = Kalman_Variables.Ptb;
    float Pbb_t = Kalman_Variables.Pbb;
    
    float uw_t = Kalman_Variables.uw;
    float ut_t = Kalman_Variables.ut;
    float ub_t = Kalman_Variables.ub;
    
    Kalman_Variables.uw = uw_t;
    Kalman_Variables.ut = ut_t + T*uw_t;
    Kalman_Variables.ub = ub_t;
    
    Kalman_Variables.Pww = Pww_t + T*qw;
    Kalman_Variables.Pwt = T*Pww_t + Pwt_t + (1/2)*(T*T)*qw;
    Kalman_Variables.Pwb = Pwb_t;
    
    Kalman_Variables.Ptt = (T*T)*Pww_t + 2*T*Pwt_t + Ptt_t + (1/3)*(T*T*T)*qw;
    Kalman_Variables.Ptb = T*Pwb_t + Ptb_t;
    
    Kalman_Variables.Pbb = Pbb_t + T*qb;
}

float Kalman_Accelerometer(double theta, uint8_t rt)
{
    float Pww_t = Kalman_Variables.Pww;
    float Pwt_t = Kalman_Variables.Pwt;
    float Pwb_t = Kalman_Variables.Pwb;
    float Ptt_t = Kalman_Variables.Ptt;
    float Ptb_t = Kalman_Variables.Ptb;
    float Pbb_t = Kalman_Variables.Pbb;
    
    float uw_t = Kalman_Variables.uw;
    float ut_t = Kalman_Variables.ut;
    float ub_t = Kalman_Variables.ub;

    float Kw = Pwt_t/(Ptt_t+rt);
    float Kt = Ptt_t/(Ptt_t+rt);
    float Kb = Ptb_t/(Ptt_t+rt);

    Kalman_Variables.uw = uw_t + Kw*(theta-ut_t);
    Kalman_Variables.ut = ut_t + Kt*(theta-ut_t);
    Kalman_Variables.ub = ub_t + Kb*(theta-ut_t);
    
    Kalman_Variables.Pww = Pww_t + (Kw*Kw)*Ptt_t + (Kw*Kw)*rt - 2*Kw*Pwt_t; 
    Kalman_Variables.Pwt = Kw*Kt*rt - (Kt - 1)*(Pwt_t - Kw*Ptt_t);
    Kalman_Variables.Pwb = Pwb_t - Kb*(Pwt_t - Kw*Ptt_t) - Kw*Ptb_t + Kw*Kb*rt;
    Kalman_Variables.Ptt = (Kt*Kt)*rt + Ptt_t*((Kt - 1)*(Kt - 1));
    Kalman_Variables.Ptb = Kt*Kb*rt - Ptb_t*(Kt - 1) + Kb*Ptt_t*(Kt - 1);
    Kalman_Variables.Pbb = Pbb_t + (Kb*Kb)*Ptt_t + (Kb*Kb)*rt - 2*Kb*Ptb_t;

    return Kalman_Variables.ut;
}

float Kalman_Gyro(double dtheta, float rw)
{
    float Pww_t = Kalman_Variables.Pww;
    float Pwt_t = Kalman_Variables.Pwt;
    float Pwb_t = Kalman_Variables.Pwb;
    float Ptt_t = Kalman_Variables.Ptt;
    float Ptb_t = Kalman_Variables.Ptb;
    float Pbb_t = Kalman_Variables.Pbb;
    
    float uw_t = Kalman_Variables.uw;
    float ut_t = Kalman_Variables.ut;
    float ub_t = Kalman_Variables.ub;

    float Kw = (Pww_t+Pwb_t)/(Pww_t+2*Pwb_t+Pbb_t+rw);
    float Kt = (Pwt_t+Ptb_t)/(Pww_t+2*Pwb_t+Pbb_t+rw);
    float Kb = (Pwb_t+Pbb_t)/(Pww_t+2*Pwb_t+Pbb_t+rw);

    Kalman_Variables.uw = uw_t+Kw*(dtheta-uw_t-ub_t);
    Kalman_Variables.ut = ut_t+Kt*(dtheta-uw_t-ub_t);
    Kalman_Variables.ub = ub_t+Kb*(dtheta-uw_t-ub_t);

    Kalman_Variables.Pww = (Kw*Kw)*rw + Kw*(Pwb_t*(Kw - 1) + Kw*Pbb_t) + (Kw - 1)*(Pww_t*(Kw - 1) + Kw*Pwb_t);
    Kalman_Variables.Pwt = Kt*(Pwb_t*(Kw - 1) + Kw*Pbb_t) - Pwt_t*(Kw - 1) + Kt*(Pww_t*(Kw - 1) + Kw*Pwb_t) - Kw*Ptb_t + Kw*Kt*rw;
    Kalman_Variables.Pwb = Kb*(Pww_t*(Kw - 1) + Kw*Pwb_t) + (Kb - 1)*(Pwb_t*(Kw - 1) + Kw*Pbb_t) + Kw*Kt*rw;
    Kalman_Variables.Ptt = Ptt_t + (Kt*Kt)*Pbb_t + 2*(Kt*Kt)*Pwb_t + (Kt*Kt)*Pww_t + (Kt*Kt)*rw - 2*Kt*Ptb_t - 2*Kt*Pwt_t;
    Kalman_Variables.Ptb = (Kb - 1)*(Kt*Pbb_t - Ptb_t + Kt*Pwb_t) + Kb*(Kt*Pwb_t - Pwt_t + Kt*Pww_t) + Kt*Kb*rw;
    Kalman_Variables.Pbb = (Kb*Kb)*rw + Kb*(Pwb_t*(Kb - 1) + Kb*Pww_t) + (Kb - 1)*(Pbb_t*(Kb - 1) + Kb*Pwb_t);
    
    return Kalman_Variables.uw;
}

void Kalman_Process(float* theta_Out, float* dtheta_Out, float theta_In, float dtheta_In)
{
    float rw = 1e-02;      //Gyro measurement noise parameter (small) dtheta trust
    float rt = 50;         //Angle measurement noise parameter (large) theta trust
    
    float qw = 10;        //Angular acceleration noise parameter (large)
    float qb = 1e-03;      //Gyro drift parameter (small) bias

    uint8_t pw = 10;        //Initial state uncertainty (large)
    uint8_t pb = 60;        //Initial state uncertainty (large)
    
    float T = 0.01;

    if(KalmanIsInit == 0)
    {
        KalmanIsInit = Kalman_Init(pw, pb, T);
    }
    else
    {
        Kalman_TimeUpdate(qw, qb, T);
        *theta_Out = Kalman_Accelerometer(theta_In, rt);
        *dtheta_Out = Kalman_Gyro(dtheta_In, rw);
    }
}