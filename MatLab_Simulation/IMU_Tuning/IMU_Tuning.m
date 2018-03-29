clear;
clc;
close all;

Data_enc = dlmread('enc4.txt',',',1,0);
theta_enc = Data_enc(:, 2)/4096 * 2 * pi;

Data_imu = dlmread('imu4.txt',',',1,0);
dtheta_imu = ((Data_imu(:, 2)*0.286)/16.4)*pi/180;
ar = Data_imu(:, 3);
at = Data_imu(:, 4);

T = 0.01;
N = 5;

%%

[~, dtheta_enc, ~] = quadraticSavitzkyGolay(theta_enc',T,N);

fileID = fopen('Kalman_Tuning_data_4.txt','w');
fprintf(fileID, '[true theta], [true dTheta], [ar], [at], [imu dtheta]\n\r');
for i = 1:1998
    fprintf(fileID,'%f, %f, %f, %f, %f\n', theta_enc(i), dtheta_enc(i), ar(i), at(i), dtheta_imu(i));
end
fclose(fileID);

%%

figure
title('dTheta');
hold on;
plot(dtheta_imu, 'r');
plot(dtheta_enc, 'b');
%

figure;
title('Theta');
hold on;
theta_imu = atan2(at, ar);
plot(theta_imu, 'b');
plot(theta_enc, 'g');
