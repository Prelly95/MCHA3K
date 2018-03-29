%% Initilise and normalise test data

close all;
clear;
clc;

data1_3s_2200mAh = dlmread('LogData1_3s_2200mAh.txt',',',1,0);
data2_3s_2200mAh = dlmread('LogData2_3s_2200mAh.txt',',',1,0);
data3_3s_2200mAh = dlmread('LogData3_3s_2200mAh.txt',',',1,0);

dataNorm1 = (data1_3s_2200mAh(:, 2));
dataNorm2 = (data2_3s_2200mAh(:, 2));
dataNorm3 = (data3_3s_2200mAh(:, 2));

totalswingtime = (0:2399)';

theta1 = dataNorm1/4096 * 2 * pi;
theta2 = dataNorm2/4096 * 2 * pi;
theta3 = dataNorm3/4096 * 2 * pi;

figure;
hold on;
grid on;
title('Swing Test');
xlabel('time(s)');
ylabel('theta(rads)');
plot(totalswingtime, theta1);
plot(totalswingtime, theta2);
plot(totalswingtime, theta3);

%% Initilise variables
t_end = 12;
t = 0: 0.005: t_end-0.005;
t = t';

samples = 2400;

theta0 = 1.09;
dtheta0 = 0;

M = 0.437;
l = 0.111;
g = 9.81;

T = 0.005;
N = 9;

%% Data Set 1 Fitting
params = leastSquares(t, theta1);

c = params(1);
I = params(2);

sim('pendulum_model_2200mAh');

theta_sim_set1 = theta_sim;

%% Data Set 2 Fitting
params = leastSquares(t, theta2);

c = params(1);
I = params(2);

sim('pendulum_model_2200mAh');

theta_sim_set2 = theta_sim;

%% Data Set 3 Fitting
params = leastSquares(t, theta3);

c = params(1);
I = params(2);

sim('pendulum_model_2200mAh');

theta_sim_set3 = theta_sim;

%% Plotting Data

I_CG = I-M*l^2;
fprintf('\nI center of gravity = %i\n', I_CG);

figure;
hold on;
grid on;

title('Swing Test With 2200mAh Battery');

figure;
hold on;
grid on;
xlim([0 t(end)]);
xlabel('Time (sec)');
ylabel('theta(rads)');
plot(tsim, theta_sim_set1,'g');
plot(t, theta1);

figure;
hold on;
grid on;
xlim([0 t(end)]);
xlabel('Time (sec)');
ylabel('theta(rads)');
plot(tsim, theta_sim_set2,'g');
plot(t, theta2);

figure;
hold on;
grid on;
xlim([0 t(end)]);
xlabel('Time (sec)');
ylabel('theta(rads)');
plot(tsim, theta_sim_set3,'g');
plot(t, theta3);