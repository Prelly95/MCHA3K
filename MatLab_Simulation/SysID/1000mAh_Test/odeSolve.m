function dtheta = odeSolve(I, c, ~, x)

M = 0.352;
l = 100;
g = 9.81;

dtheta = [dtheta; (1/I)*(-c*dtheta-M*l*g*sin(theta))];
