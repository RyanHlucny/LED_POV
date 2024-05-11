% Ball & Beam System
% ECE 463 HW04
% Ryan Hlucny

X = [0, 0, 0, 0]';
dt = 0.01;
t = 0;
n = 0;
y = [];
Ref = 1;

Kx = [-53.3986  211.7196  -38.1709   58.0000];
Kr = -23.9986;

while(t < 10)
 F = Kr*Ref - Kx*X;
 T = 0;
 dX = BallBeamDynamics(X, F, T);

 X = X + dX * dt;
 t = t + dt;
 
 y = [y ; Ref, X(1)];
 n = mod(n+1,5);
 if(n == 0)
    BallBeamDisplay(X, Ref);
 end
 end
 
t = [1:length(y)]' * dt;

plot(t,y(:,1),'r',t,y(:,2),'b');
xlabel('Time (seconds)');
ylabel('Ball Position');

