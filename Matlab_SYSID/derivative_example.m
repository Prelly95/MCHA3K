% Example demonstrating derivative estimation
close all;
clc;

T = 0.01;

g = 9.81;
m = 0.2;


theta = Anglerad';
t = Timesec';

ts = timeseries(theta', t');

N = 9; % 2*N+1 points are used as local neighbourhood for quad fit

% Central difference derivatives

dtheta_diff = nan(size(t));
ddtheta_diff = nan(size(t));
for k = 1+N:length(t)-N
    dtheta_diff(k) = (theta(k+1) - theta(k-1))/(2*T);
    ddtheta_diff(k) = (theta(k+1) - 2*theta(k) + theta(k-1))/T^2;
end

% Filtered derivatives
[~,dtheta_quad, ddtheta_quad] = quadraticSavitzkyGolay(theta,T,N);

sinA = sin(theta);

A = [dtheta_quad((N+1):1001-(N+1))' sinA((N+1):1001-(N+1))'];
X = A\ddtheta_quad((N+1):1001-(N+1))';

l = -g/X(2);
c = -X(1)*(m*l^2);

sim('pendulum_mode')

plot(t,theta_sim,'b',t,theta,'g')
xlim([0 t(end)]);
xlabel('Time (sec)')
title('First derivative')
grid on