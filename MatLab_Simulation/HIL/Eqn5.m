function pTheta = Eqn5(in)
global param;

theta = in(1);
dtheta = in(2);
dphi = in(3);


M = param.M;
m = param.m;
l = param.l;
r = param.r;
J = param.J;
I = param.I;

%pTheta = (dtheta+dphi)*(J+(M+m)*r^2)+dphi*M*l*r*cos(theta)+dtheta*(2*M*l*r*cos(theta)+I+M*l^2);

pTheta = dphi*(J+(M+m)*r^2 + M*l*r) + dtheta*((J+(M+m)*r^2)+2*M*l*r+I+M*l^2);