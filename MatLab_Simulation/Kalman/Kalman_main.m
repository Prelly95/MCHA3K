clear;
clc;
close all;

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

Data = dlmread('Kalman_Tuning_data_4.txt',',',1,0);

enc_Theta = Data(:, 1);
enc_dTheta = Data(:, 2);
ar = Data(:, 3);
at = Data(:, 4);
imu_dTheta = Data(:, 5);
imu_Theta = atan2(at, ar);

figure;
hold on;
plot(at);
plot(ar);
% plot(imu_theta);
% plot(enc_Theta);

%%

W = zeros(1, 1988);
Th = zeros(1, 1988);

for i = 1:1988
      [Th(i), W(i)] = Kalman_Filter([imu_Theta(i), imu_dTheta(i)]);
end

figure;
hold on;
plot(enc_Theta, 'r');
plot(Th, 'b');

figure;
hold on;
plot(enc_dTheta, 'r');
plot(W, 'b');