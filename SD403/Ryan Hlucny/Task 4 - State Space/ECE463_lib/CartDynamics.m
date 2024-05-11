function [ dX ] = CartDynamics(X, F, T)
% Cart Dynamics
% ECE 463 HW04
% Ryan Hlucny
% X = [x, q, dx, dq]
x = X(1);
q = X(2);
dx = X(3);
dq = X(4);

g = 9.8;
L = 1.3;
m1 = 4;
m2 = 1;
S = @(q) sin(pi/2-q);
C = @(q) cos(pi/2-q);

% Nonlinear Dynamics
M = [m1+m2 L*m2*S(q); L*m2*S(q) m2*L^2];
A = [L*m2*C(q)*dq*dq; L*g*m2*C(q)];
B1 = [1;0];
B2 = [0;1];
d2X = inv(M) * (A + B1*F + B2*T);
ddx = d2X(1);
ddq = d2X(2);

dX = [dx; dq; ddx; ddq];
end