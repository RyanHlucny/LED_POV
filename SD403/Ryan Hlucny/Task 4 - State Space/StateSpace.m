% SD_403_Sp24_06 LED Spinner
% Ryan Hlucny Task 4
% State Space Observer

addpath('.\ECE463_lib\');

m = 0.0453592; 
r = 0.3048; % 1 foot
 
A = [0 1; 0 0]
B = [0; 6/(m*r^2)]
C = [1 0; 0 1];
D = zeros(2,1);
eig(A)
G = ss(A,B,C,D)

%% Observer Gains

Q = diag([1e4,1e2]);
R = 1;
H = lqr(A', C', Q, R)'

% Augmented system (Plant + Observer)
A4 = [A zeros(2,2); H*C (A-H*C)]
B4 = [B;B]
C4 = [1 0 0 0; 0 1 0 0; 0 0 1 0; 0 0 0 1];
D4 = zeros(4,1);
t = linspace(0,0.5,1e3);
X0 = [0;0;10*rand;10*rand];
U = ones(size(t)).*(t<0.25);
y = step3(A4, B4, C4, D4, t, X0, U);
figure(1); clf(1); set(gcf, "Name", "Linear Simulation");
tiledlayout('vertical'); nexttile;
plot(t,y(:,1)); grid on; title("\theta(t)"); nexttile;
plot(t,y(:,2)); grid on; title("\omega(t)"); 

figure(2); clf(2); set(gcf, "Name", "Linear Simulation with Observer");
tiledlayout('vertical'); nexttile;
plot(t,y(:,[1,3])); grid on; title("\theta(t)"); 
legend(["Plant", "Observer"]); nexttile;
plot(t,y(:,[2,4])); grid on; title("\omega(t)"); 
legend(["Plant", "Observer"]);

%% Nonlinear Simulation

X = [0;0];
Xe = [rand;rand];
t = 0;
dt = 0.0005;
T = 1;
figure(3); clf(3); set(gcf, "Name", "Nonlinear Simulation");
q = X(1);
dq = X(2);
y = [];
while (t < 0.5)
    T = 1.*(t<0.25); % Only apply  torque for the first 0.1 seconds
    
    % Dynamics
    ddq = 6/(m*r^2)*T; 
    dq = dq + ddq*dt
    q = q + dq*dt;
    X(1) = q;
    X(2) = dq;

    % Observer dynamics
    dXe = H*C*X + (A-H*C)*Xe+B*T
    Xe = Xe + dXe*dt

    t = t + dt
    y = [y; X(1), X(2), Xe(1), Xe(2), T];

    % Animation
    plot([-r*cos(q), r*cos(q)],[-r*sin(q), r*sin(q)],'ro'); hold on;
    line([-r*cos(q), r*cos(q)],[-r*sin(q), r*sin(q)],"Color","r");
    plot([-r*cos(Xe(1)), r*cos(Xe(1))], [-r*sin(Xe(1)), r*sin(Xe(1))], 'go'); 
    line([-r*cos(Xe(1)), r*cos(Xe(1))],[-r*sin(Xe(1)), r*sin(Xe(1))],"Color","g"); hold off;
    grid on; axis equal; xlim([-1 1]); ylim([-1 1]); legend({'', 'Plant','','Observer'});
    pause(0.01)
end
hold off;
t = [1:length(y)]' * dt;
clf(3); tiledlayout('vertical'); nexttile;
plot(t,y(:,[1,3])); grid on; title('\theta(t)'); 
legend(["Plant","Observer"]); nexttile; 
plot(t,y(:,[2,4])); grid on; title('\omega(t)'); 
legend(["Plant","Observer"])