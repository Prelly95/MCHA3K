function [x,dx,ddx] = quadraticSavitzkyGolay(x0,T,N)

x = nan(size(x0));
dx = nan(size(x0));
ddx = nan(size(x0));

for k = 1+N:length(x0)-N
    % Local quadratic fit and derivative estimation from coefficients
    p = polyfit((-N:N)*T,x0(k-N:k+N),2);
    x(k) = p(3);
    dx(k) = p(2);
    ddx(k) = 2*p(1);
end