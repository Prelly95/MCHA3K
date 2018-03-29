
global param;

% Cart/pendulum parameters
param.M = 1;              % Mass of cart (kg)
param.m = 0.5;          	% Mass of pendulum (kg)
param.l = 0.5;            % Length of pendulum (m)
param.J = 3.22e-06;
param.I = 78.125e-06;
param.r = 0.045;
param.b = 0.01;           % Damping of cart (N.s/m)
param.c = 0.05;           % Damping of pendulum (N.m.s/rad)

% Other parameters
param.g = 9.82;           % Acceleration due to gravity (m/s/s)
param.alpha = 1*pi/180;   % Ground slope (rad = deg*pi/180)
theta0 = 5*pi/180;  % Initial angle (rad = deg*pi/180)

% Inner (angle) controller parameters
p = 5.4277;
p1 = 25.4277;
K1 = -119.0071;

% Outer (velocity) controller parameters
K2 = 0.0402;


% Run simulation
tsim = 1;
T = 0.01;
set_param('pendcarthil','AlgebraicLoopSolver','LineSearch');
sim('pendcarthil');

% Plot results
figure(1);clf
subplot(2,2,1)
%plot(t,theta*180/pi,t,ri*180/pi,'r')
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('\theta (\circ)')
legend('Actual','Demanded','Location','Best')

subplot(2,2,2)
plot(t,omega*180/pi)
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('\omega (\circ/s)')

subplot(2,2,3)
plot(t,omega)
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('x (m)')

subplot(2,2,4)
plot(t,omega,t,r,'r')
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('v (m/s)')
legend('Actual','Demanded','Location','Best')

figure(2);clf
plot(t,u)
grid on
title('Control force')
xlabel('Time (sec)')
ylabel('Force (N)')




