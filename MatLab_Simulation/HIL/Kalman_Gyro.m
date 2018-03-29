function [Corrected_dTheta] = Kalman_Gyro(dtheta, rw)

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

Kw = (Pww+Pwb)/(Pww+2*Pwb+Pbb+rw);
Kt = (Pwt+Ptb)/(Pww+2*Pwb+Pbb+rw);
Kb = (Pwb+Pbb)/(Pww+2*Pwb+Pbb+rw);

Kalman_Variables.uw = uw+Kw*(dtheta-uw-ub);
Kalman_Variables.ut = ut+Kt*(dtheta-uw-ub);
Kalman_Variables.ub = ub+Kb*(dtheta-uw-ub);

Kalman_Variables.Pww = (Kw^2)*rw + Kw*(Pwb*(Kw - 1) + Kw*Pbb) + (Kw - 1)*(Pww*(Kw - 1) + Kw*Pwb);
Kalman_Variables.Pwt = Kt*(Pwb*(Kw - 1) + Kw*Pbb) - Pwt*(Kw - 1) + Kt*(Pww*(Kw - 1) + Kw*Pwb) - Kw*Ptb + Kw*Kt*rw;
Kalman_Variables.Pwb = Kb*(Pww*(Kw - 1) + Kw*Pwb) + (Kb - 1)*(Pwb*(Kw - 1) + Kw*Pbb) + Kw*Kt*rw;
Kalman_Variables.Ptt = Ptt + Kt^2*Pbb + 2*Kt^2*Pwb + Kt^2*Pww + Kt^2*rw - 2*Kt*Ptb - 2*Kt*Pwt;
Kalman_Variables.Ptb = (Kb - 1)*(Kt*Pbb - Ptb + Kt*Pwb) + Kb*(Kt*Pwb - Pwt + Kt*Pww) + Kt*Kb*rw;
Kalman_Variables.Pbb = Kb^2*rw + Kb*(Pwb*(Kb - 1) + Kb*Pww) + (Kb - 1)*(Pbb*(Kb - 1) + Kb*Pwb);

Corrected_dTheta = Kalman_Variables.uw;
