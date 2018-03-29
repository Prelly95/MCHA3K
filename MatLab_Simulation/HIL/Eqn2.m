function dTheta = Eqn2(in)
global param;

theta  = in(1);
dphi   = in(2);
Ptheta = in(3);
alpha  = in(4);

% Cart/pendulum parameters
M = param.M;
m = param.m;
l = param.l;
r = param.r;
J = param.J;
I = param.I;

% top = Ptheta - dphi*(J + (M+m)*r^2 + M*l*r);
% bottom = J+(M+m)*r^2 + 2*M*l*r + M*l^2 + I;

top = Ptheta - dphi*(J+(M+m)*r^2 + M*l*r*cos(theta+alpha));
bottom = J + (M+m)*r^2 + 2*M*l*r*cos(theta+alpha) + I + M*l^2;

dTheta = top/bottom;



