 function [dX] = BallBeamDynamics( X, T, F )
% BallBeamDynamics(X, T);
% ECE 463 HW04
% Ryan Hlucny

r = X(1);
q = X(2);
dr = X(3);
dq = X(4);

g = 9.8;
m = 3;
J = 2;
S = @(q) sin(q);
C = @(q) cos(q);

M = [7/5*m 0; 0 m*r*r+J];
A = [m*r*dq*dq-m*g*S(q); -2*m*r*dr*dq-g*m*C(q)*r];
B1 = [1;0];
B2 = [0;1];

ddX = inv(M)*(A+B1*F+B2*T);
ddr = ddX(1);
ddq = ddX(2);

dX = [dr; dq; ddr; ddq];

end