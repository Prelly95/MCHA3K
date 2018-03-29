clc;
close all;

DataIMU = dlmread('IMUVidData.txt',',',1,0);
DataVid = dlmread('theta.txt');
t = dlmread('Time.txt');

aX = DataIMU(:, 1);
aY = DataIMU(:, 2)/16384;
aZ = DataIMU(:, 3);

rX = DataIMU(:, 4)/16400;
rY = DataIMU(:, 5);
rZ = DataIMU(:, 6);

td = 1:0.0334:67.77;
figure;
hold on;
plot(t, DataVid, 'r');
plot(td, -rX);