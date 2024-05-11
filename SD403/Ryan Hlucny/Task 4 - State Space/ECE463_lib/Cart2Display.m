function [] = Cart2Display(X, Xe, Ref)
% Test #3 for Spring 2024
% Double pendulum with first link pointing up
% second link pointing down

% Observer
x1 = Xe(1);
y1 = 0.2;
q1 = Xe(2);
q2 = Xe(3) + pi;
% cart
X1 = [-0.2,0.2,0.2,-0.2,-0.2] + x1;
Y1 = [0,0,0.2,0.2,0];
x2 = x1 + sin(q1);
y2 = y1 + cos(q1);
x3 = x2 + sin(q1+q2);
y3 = y2 + cos(q1+q2);
X2 = [x1, x2,x3];
Y2 = [y1, y2,y3];
X3 = [-3,3];
Y3 = [0,0];
% ball
q = [0:0.1:1]' * 2*pi;
xb = 0.05*cos(q);
yb = 0.05*sin(q);

hold off
plot(X1,Y1,'m',X2,Y2,'m',xb+x2, yb+y2, 'm', xb+x3, yb+y3, 'm');
axis equal;
% ylim([-0.5,5]);
hold on

x1 = X(1);
y1 = 0.2;
q1 = X(2);
q2 = X(3) + pi;
% cart
X1 = [-0.2,0.2,0.2,-0.2,-0.2] + x1;
Y1 = [0,0,0.2,0.2,0];
x2 = x1 + sin(q1);
y2 = y1 + cos(q1);
x3 = x2 + sin(q1+q2);
y3 = y2 + cos(q1+q2);
X2 = [x1, x2,x3];
Y2 = [y1, y2,y3];
X3 = [-3,3];
Y3 = [0,0];
% ball
q = [0:0.1:1]' * 2*pi;
xb = 0.05*cos(q);
yb = 0.05*sin(q);

plot(X1,Y1,'r',X2,Y2,'r',X3,Y3,'b',xb+x2, yb+y2, 'r', xb+x3, yb+y3, 'r',[Ref, Ref],[-0.1,0.1],'b')
% ylim([-0.5,2.5]);
pause(0.01);
end