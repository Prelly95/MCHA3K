%% Initilise and normalise test data
close all;
clear;
clc;

Data1 = dlmread('Data1.txt',',',1,0);
Data2 = dlmread('Data2.txt',',',1,0);
Data3 = dlmread('Data3.txt',',',1,0);

t = 0: 0.005: 10.085;
t = t';

figure;
hold on;
title('Raw Data');
plot(t, Data1(:, 2)-655, 'r');
plot(t, Data2(:, 2)-680, 'g');
plot(t, Data3(:, 2)-689, 'b');

theta1 = Data1(:, 2)-655;
theta2 = Data2(:, 2)-680;
theta3 = Data3(:, 2)-689;

totalswingtime = Data1(:, 1);

theta1 = theta1/4096 * 2 * pi;
theta2 = theta2/4096 * 2 * pi;
theta3 = theta3/4096 * 2 * pi;

figure;
hold on;
grid on;
title('Swing Test');
xlabel('time(s)');
ylabel('theta(rads)');
plot(t, theta1);
plot(t, theta2);
plot(t, theta3);

%% Initilise variables

samples = 2018;

theta0 = 0.83;
dtheta0 = 0;

M = 0.330;
len = 0.093;
g = 9.81;

T = 0.005;
N = 9;

%% Data Set 1 Fitting
params = leastSquares(t, theta1);

% c = params(1);
% I = params(2);

c = 1.83e-04;
I = 4.4e-03;

sim('pendulum_model_1000mAh');

theta_sim_set1 = theta_sim;

%%Plotting Data

I_CG = I-M*len^2;

fprintf('\nI center of gravity = %i\n', I_CG);

figure;
hold on;
grid on;
% title('Swing Test With 1000mAh Battery');

xlabel('Time (sec)');
ylabel('theta(rads)');
plot(tsim, theta_sim_set1,'k');
plot(t, theta1, 'r');
plot(t, theta2, 'g');
plot(t, theta3, 'b');