function dPTheta = Eqn1(in)

theta  = in(1);
dtheta = in(2);
dphi   = in(3);
alpha  = in(4);

global param;

% Cart/pendulum parameters
M = param.M;
m = param.m;
l = param.l;
r = param.r;
g = param.g;

dPTheta = M*l*g*sin(theta)-((M+m)*g*r*sin(alpha))-M*l*r*sin(theta+alpha)*(dphi+dtheta);