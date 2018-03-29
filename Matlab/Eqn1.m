function vdd = Eqn1(in)

%u = theta
%v=  phi

u     = in(1);
ud    = in(2);
udd   = in(3);
tau   = in(4);
alpha = in(5);

global param;

% Cart/pendulum parameters
M = param.M;
m = param.m;
l = param.l;
r = param.r;
J = param.J;
g = param.g;


top = tau - udd*(J+(M+m)*r^2)-M*r*l*udd*cos(u+alpha)-M*l*ud^2*sin(u+alpha)-(M+m)*r*g*sin(alpha);
bottom = (J+(M+m)*r^2);

vdd = top/bottom;