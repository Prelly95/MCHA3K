function [Th, W] = Kalman_Filter(in)

global Kalman_Variables;
theta = in(1);
dtheta = in(2);

% Kalman Vatiables
rw = 1e-05;
rt = 100;

qw = 200;
qb = 1e-05;

T = 0.01;

pw = 10;
pb = 60;

if (Kalman_Variables.Pww == 0)
    Kalman_Init(pw, pb, T);
    disp('init');
end

Kalman_TimeUpdate(qw, qb, T);
Th = Kalman_Accelerometer(theta, rt);
W = Kalman_Gyro(dtheta, rw);

