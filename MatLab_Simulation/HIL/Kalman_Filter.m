function [out] = Kalman_Filter(in)

global Kalman_Variables;

theta = in(1);
dtheta = in(2);

% Kalman Vatiables
rw = 1e-02;
rt = 50;

qw = 10;
qb = 1e-03;

T = 0.01;

pw = 10;
pb = 60;

if (Kalman_Variables.Pww == 0)
    Kalman_Init(pw, pb, T)
end


Kalman_TimeUpdate(qw, qb, T);
Th = Kalman_Accelerometer(theta, rt);
W = Kalman_Gyro(dtheta, rw);

out(1,1) = Th;
out(2,1) = W;
