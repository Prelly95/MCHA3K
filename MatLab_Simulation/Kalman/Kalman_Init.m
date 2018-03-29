function Kalman_Init(pw, pb, T)

global Kalman_Variables;

Kalman_Variables.Pww = T*pw;
Kalman_Variables.Pwt = (1/2)*T^2*pw;
Kalman_Variables.Pwb = 0;

Kalman_Variables.Ptt = (1/3)*T^3*pw;
Kalman_Variables.Ptb = 0;

Kalman_Variables.Pbb = T*pb;

Kalman_Variables.uw = 0;
Kalman_Variables.ut = 0;
Kalman_Variables.ub = 0;