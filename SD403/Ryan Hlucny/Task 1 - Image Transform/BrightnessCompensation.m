q = linspace(0, 2*pi, 1e2);

a = 1; % Top height
b = 0.5; % Bottom height
x = cos(q);
y = sin(q);
[X,Y] = meshgrid(x,y);
Z = @(x,y) (b-a)*(sqrt(x.^2+y.^2))+a;

surf(x,y,Z(X,Y));