p = 5.4277;
p1 = 25.4277;
K1 = -119.0071;

% Outer (velocity) controller parameters
K2 = 0.0402;


%% Convert two continuous-time SISO controllers in cascade architecture to one discrete-time state-space system
C1c = tf([K1, K1*p],[1 p1]); % Inner loop tf
C2c = tf(K2);    % Outer loop tf

T = 0.01; % sampling time
C1d = c2d(ss(C1c),T);
C2d = c2d(ss(C2c),T);

[A1,B1,C1,D1] = ssdata(C1d);
[A2,B2,C2,D2] = ssdata(C2d);

% The input vector to this system is u = [vref;v;theta]
% The output from this system is y = F

A = single([A1, B1*C2; zeros(size(A2,1),size(A1,1)), A2]);
B = single([B1*D2, -B1*D2, -B1; B2, -B2, zeros(size(A2,1),1)]);
C = single([C1, D1*C2]);
D = single([D1*D2, -D1*D2, -D1]);

%% Simulate controller to obtain verification data
Cd = ss(A,B,C,D,T);

n = size(A,1); % number of controller states
t = single(0:T:(n+1)*T);

v_ref = 1 + 0.1*t;
v = -2 + 0.2*t;
theta = 0 + 0.01*t;

u = single([v_ref; v; theta]);
x0 = single(0.1*ones(n,1));

[y,t_,x] = lsim(Cd, u.', t, x0);

%% Print verification data to be used for native testing
format short g

% Controller matrices
A, B, C, D

% Test data
x0
u
y

% One step test data
u(:,1)
A*x0, B*u(:,1), C*x0, D*u(:,1)

%
% Pro automation tip:
%
% To avoid copying, pasting and reformatting these results every
% time you redesign your controller, use fprintf to format
% this data as C array initialiser lists and write to file(s) that
% you can #include in the controller and unit test modules.
%
% Don't forget to update the CTRL_N_INPUT, CTRL_N_STATE and
% CTRL_N_OUTPUT enum constants
%