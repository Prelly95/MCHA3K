function PARAMS = leastSquares(~, theta)
M = 0.437;
l = 0.111;
g = 9.81;

T = 0.005;
N = 9;

samples = 2400;


[~, dtheta, ddtheta] = quadraticSavitzkyGolay(theta',T,N);

phi = [dtheta((N+1):samples-(N+1))' ddtheta((N+1):samples-(N+1))'];
y = -M*l*g*sin(theta((N+1):samples-(N+1)));

PARAMS = lsqr(phi, y);