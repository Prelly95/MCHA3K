clear all;
clc;

close all;

M = 0.330;
m = 0.508;
l = 0.095;
J = 3.22e-06;
I = 1.545830e-03;
r = 0.045;
g = 9.81;

T = 0.01;

% Kalman Vatiables
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

theta0 = 0;
Ptheta0 = 0;
stepInput = 1;
simTime = 10;

A = [   
        0,  M*l*g, 0
        1/(J+(M+m)*r^2+2*M*l*r+I+M),  0, -(J+(M+m)*r^2+M*l*r)/(J+(M+m)*r^2+2*M*l*r+I+M*l^2)
        0, 0, 0
    ];

eig(A)

B = [
        0
        0
        1
    ];

C = [
        0, 1, 0
        1/(J+(M+m)*r^2+2*M*l*r+I+M),  0, -(J+(M+m)*r^2+M*l*r)/(J+(M+m)*r^2+2*M*l*r+I+M*l^2)
        0, 0, 1
    ];

D = [
        0
        0
        0
    ];

K = acker(A, B, [-6+10i, -6-10i, -8]);
Kz =350;

eig(A-B*[K(1) K(2) K(3)])

%Controller statespace
Ac = [
        0, -r
        -Kz, -K(1)*(J+(M+m)*r^2 + M*l*r)-K(3)
     ];
Bc = [
        0, -r, 1
        -K(2), -K(1)*((J+(M+m)*r^2)+2*M*l*r+I+M*l^2), 0
     ];
 
Cc = [
        0, 1
     ];
 
Dc = [
        0, 0, 0
     ];
 
 SYS_C = ss(Ac, Bc, Cc, Dc);
 
[SYS_D] = c2d(SYS_C, T);

[Ad, Bd, Cd, Dd] = ssdata(SYS_D);

fileID = fopen('ctrl\A.def','w');
fprintf(fileID, '{%f, %f}, {%f, %f}', Ad(1, 1), Ad(1, 2), Ad(2, 1), Ad(2, 2));
fclose(fileID);

fileID = fopen('ctrl\B.def','w');
fprintf(fileID, '{%f, %f, %f}, {%f, %f, %f}', Bd(1, 1), Bd(1, 2), Bd(1, 3), Bd(2, 1), Bd(2, 2), Bd(2, 3));
fclose(fileID);

fileID = fopen('ctrl\C.def','w');
fprintf(fileID, '{%f, %f}', Cd(1, 1), Cd(1, 2));
fclose(fileID);

fileID = fopen('ctrl\D.def','w');
fprintf(fileID, '{%f, %f, %f}', Dd(1, 1), Dd(1, 2), Dd(1, 3));
fclose(fileID);
