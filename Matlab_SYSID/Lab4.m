% Example demonstrating derivative estimation
close all;
clc;

T = 0.01;

F = ForceN';
V = Velocityms';
t = Timesec';

ts = timeseries(F', t');

N = 3; % 2*N+1 points are used as local neighbourhood for quad fit

% Central difference derivatives

dV_diff = nan(size(t));
ddV_diff = nan(size(t));
for k = 1+N:length(t)-N
    dV_diff(k) = (V(k+1) - V(k-1))/(2*T);
    ddV_diff(k) = (V(k+1) - 2*V(k) + V(k-1))/T^2;
end

% Filtered derivatives
[~,dV_quad, ddV_quad] = quadraticSavitzkyGolay(V,T,N);

A = [F((N+1):1001-(N+1))' V((N+1):1001-(N+1))'];
X = A\dV_quad((N+1):1001-(N+1))';

M = 1/X(1);
b = -X(2)/m;

figure;

subplot(1,2,1)
plot(t,dV_diff,'b',t,dV_quad, 'g')
xlim([0 t(end)]);
grid on

subplot(1,2,2)
plot(t,ddV_diff,'b',t,ddV_quad, 'g')
xlim([0 t(end)]);
xlabel('Time (sec)')
title('Second derivative')
grid on