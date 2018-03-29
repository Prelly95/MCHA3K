function  [Corrected_Theta] = Kalman_Accelerometer(theta, rt)

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

Kw = Pwt/(Ptt+rt);
Kt = Ptt/(Ptt+rt);
Kb = Ptb/(Ptt+rt);

Kalman_Variables.uw = uw + Kw*(theta-ut);
Kalman_Variables.ut = ut + Kt*(theta-ut);
Kalman_Variables.ub = ub + Kb*(theta-ut);

Kalman_Variables.Pww = Pww + (Kw^2)*Ptt + (Kw^2)*rt - 2*Kw*Pwt; 
Kalman_Variables.Pwt = Kw*Kt*rt - (Kt - 1)*(Pwt - Kw*Ptt);
Kalman_Variables.Pwb = Pwb - Kb*(Pwt - Kw*Ptt) - Kw*Ptb + Kw*Kb*rt;
Kalman_Variables.Ptt = (Kt^2)*rt + Ptt*((Kt - 1)^2);
Kalman_Variables.Ptb = Kt*Kb*rt - Ptb*(Kt - 1) + Kb*Ptt*(Kt - 1);
Kalman_Variables.Pbb = Pbb + (Kb^2)*Ptt + (Kb^2)*rt - 2*Kb*Ptb;


Corrected_Theta = Kalman_Variables.ut;


