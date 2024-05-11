function [ dX ] = Cart2Dynamics( X, F, T1, T2 )
% Test #3 for Spring 2024
% Double pendulum with first link pointing up
% second link pointing down

x = X(1);
q1 = X(2);
q2 = X(3) + pi;
dx = X(4);
dq1 = X(5);
dq2 = X(6);
g = -9.8;

c1 = cos(q1);    
s1 = sin(q1);
c2 = cos(q2);    
s2 = sin(q2);
c12 = cos(q1 + q2);
s12 = sin(q1 + q2);

M = [3,         2*c1+c12,  c12;  
     2*c1+c12,  3+2*c2,    1+c2;
     c12,       1+c2,      1];
 
     
C = [2*s1*dq1*dq1 - s12*(dq1+dq2)^2;
    2*s1*dx*dq1 + s2*(dq1+dq2)*dq2 + s2*dq1*dq2 - 2*s1*dx*dq1;
    s2*dq1*dq2 - s2*dq1*(dq1+dq2)];
    
G = [0 ;  2*s1 + s12 ; s12];
 
ddX = inv(M)*(C - g*G + [F ; T1; T2] );

dX = [dx; dq1; dq2; ddX];

end