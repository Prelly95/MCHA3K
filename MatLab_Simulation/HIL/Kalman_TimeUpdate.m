function Kalman_TimeUpdate(qw, qb, T)

global Kalman_Variables;

Pww = Kalman_Variables.Pww;
Pwt = Kalman_Variables.Pwt;
Pwb = Kalman_Variables.Pwb;
Ptt = Kalman_Variables.Ptt;
Ptb = Kalman_Variables.Ptb;
Pbb = Kalman_Variables.Pbb;

uw = Kalman_Variables.uw;
ut = Kalman_Variables.ut;
ub = Kalman_Variables.ub;

Kalman_Variables.uw = uw;
Kalman_Variables.ut = ut + T*uw;
Kalman_Variables.ub = ub;

Kalman_Variables.Pww = Pww + T*qw;
Kalman_Variables.Pwt = T*Pww + Pwt + (1/2)*(T^2)*qw;
Kalman_Variables.Pwb = Pwb;

Kalman_Variables.Ptt = (T^2)*Pww + 2*T*Pwt + Ptt + (1/3)*(T^3)*qw;
Kalman_Variables.Ptb = T*Pwb + Ptb;

Kalman_Variables.Pbb = Pbb + T*qb;
