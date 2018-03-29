function udd = Eqn2(in)

%u = theta
%v=  phi

u     = in(1);
ud    = in(2);
vdd   = in(3);
tau   = in(4);
alpha = in(5);

global param;

% Cart/pendulum parameters
M = param.M;
m = param.m;
l = param.l;
b = param.b;
J = param.J;
r = param.r;
g = param.g;
I = param.I;

top =tau-b*ud-(J+(M+m)*r^2)*vdd-M*l*r*cos(u+alpha)*vdd-((M+m)*g*r*sin(alpha))-M*g*r*sin(u);
bottom = (J+(M+m)*r^2)+2*M*r*l*cos(u+alpha)+(I+M*l^2);

udd = top/bottom;