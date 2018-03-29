function ar = Eqn4(in)

theta  = in(1);
dtheta = in(2);
ddtheta = in(3);
ddphi = in(4);

global param;

l = param.l;
g = param.g;
r = param.r;

ddy = (-ddtheta*l*sin(theta)+dtheta^2*l*cos(theta)-g);
ddx = (ddtheta*l*cos(theta)-dtheta^2*l*sin(theta)+r*(ddtheta + ddphi));

ar = ((ddy)*cos(theta)+(ddx)*sin(theta));
