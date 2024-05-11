% Cart and Inverted Pendulum
% ECE 463 HW04
% Ryan Hlucny

X = [0; 0; 0; 0];
dX = zeros(4,1);
Ref = 1;
dt = 0.01;
t = 0;
Kx = [-17.8110 -292.3904  -28.3292  -97.1518];
Kr = -17.8110;

y = [];
while(t < 10)
 Ref = 1;
 F = Kr*Ref - Kx*X;
 T = 0;
 dX = CartDynamics(X, F, T);
 X = X + dX * dt;
 t = t + dt;
 
 CartDisplay(X, Ref);
 y = [y ; X(1), Ref];
end

t = [1:length(y)]' * dt;
plot(t,y); grid on;
max(y)

