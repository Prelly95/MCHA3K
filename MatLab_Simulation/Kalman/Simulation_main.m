clear;
clc;
close all;

M = 0.330;
m = 0.508;
l = 0.095;
J = 3.22e-06;
I = 1.457e-03;
r = 0.045;
g = 9.81;

global Kalman_Variables;

Kalman_Variables.Pww = 0;
Kalman_Variables.Pwt = 0;
Kalman_Variables.Pwb = 0;

Kalman_Variables.Ptt = 0;
Kalman_Variables.Ptb = 0;

Kalman_Variables.Pbb = 0;

Kalman_Variables.uw = 0;
Kalman_Variables.ut = 0;
Kalman_Variables.ub = 0;

global param;

param.M = M;
param.m = m;
param.l = l;
param.J = J;
param.I = I;
param.r = r;
param.g = g;

theta0 = 0.1;
Ptheta0 = 0;
stepInput = 0;
simTime = 10;

A = [   
        0,  M*l*g, 0
        1/(J+(M+m)*r^2+2*M*l*r+I+M),  0, -(J+(M+m)*r^2+M*l*r)/(J+(M+m)*r^2+2*M*l*r+I+M*l^2)
        0, 0, 0
    ];

B = [
        0
        0
        1
    ];

C = [
    
    ];

K = acker(A, B, [-10 -10 -10]);

% Kalman Vatiables
rw = 0.1;
rt = 100;

qw = 100;
qb = 0.1;

pw = 1;
pb = 50;

T = 0.01;

