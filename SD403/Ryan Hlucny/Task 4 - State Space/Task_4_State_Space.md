# HW07 - Task 4 - State Space

## Ryan Hlucny

### Task Overview

The purpose of Task 4 is to create a state space model of the system in order to create an observer for the angle state of the system. This is going to then be used by the rest of the code to do the LUT indexing for LED array switching. The advantage of doing a state space model rather than merely euler integration techniques is the resulting estimate is much more accurate and gives us the benefit of sensor fusion. With sensor fusion, we can fuse multiple sensor sources together, taking the pros of each and leaving the cons. We are using an [ICM 20948](https://www.sparkfun.com/products/15335) as the IMU (inertial measurement unit) for this project. The imu is placed as centered as possible on the axis of rotation. From the IMU, we can get angle information from the gyroscope, but we can also get acceleration data from the accelerometer. Both of these sensors are integrated into the IMU, and we can access them independently. This will allow us to take both measurements into a sensor-fusion based observer using a C matrix with two outputs, one for each state. Additionally, this allows us to reject disturbances and noise a lot better from individal sensors. IF we know a particular reading is noisy, we can reduce its weight in a Kalman filter, giving us an "optimal" observer. Overall there are many benefits to the state space model, and it's also just really cool and fun to apply one of my favorite topics from school.

### The System

Here is a screen cap of my derivation of the state space system:

![ss1](/SD403/Media/Images/SSDerivation1.png)

![ss2](/SD403/Media/Images/SSDerivation2.png)

In my analysis I forewent the forces of friction and wind resistence. This is an innacuracy in my model and is definitely an area to improve/expand upon in the future.

Pluggin in the system into MATLAB, I was able to plot the response of the system to a unit step of torque.

```matlab
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
```

I tried to pick values of m and r that were representative of our actual machine. I was surprised to see how sharp the response was. But it makes sense since the blades are so light and 1 Nm of torque is actually a lot for such a small mass. Coupled with the lack of friction and this response makes perfect sense.

![state_Space_linear](/SD403/Media/Images/StateSpaceLinearSim.png)

We see that as time advances, the system evolves as we would expect. With no friction forces and a constant torque, the mass has a constant angular acceleration, which is why we see the curve in the plot of $\theta(t)$ and the staight line in $\omega(t)$. I turned the torque off after 0.25 seconds to simulate letting the device go into free-spin after a short period of acceleration. And we see here that the system decays into a constant $\omega(t)$ and linear $\theta(t)$, as expected.

### The Observer

The observer is designed using LQR techniques. I chose Q so that the angle state would have a much greater importance than the angular velocity state. This is because I am much more concerned with knowing the angle than I am its rate of change. The entire project is solely depending on the machine's ability to detect its angle state over time accurately.

#### Linear Simulation

```matlab
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
```

Running the above code yields a useful and important plot, showing the plant and observer states over time in this linear simulation. I gave the observer a randomly selected starting position to show that the observer tracks.

![ssWithObserver](/SD403/Media/Images/StateSpaceLinearSimWithObserver.png)

And the observer converges very quickly to the actual states.

#### Nonlinear Simulation

For the nonlinear simulation I wrote the dynamics equations into MATLAB and Euler integrated twice to get the system states. I am also animating the states while it's simulating to give a good visualization of what's happening with the system.

```matlab
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
```

I also started the observer states in a randomly chosen position for this demo. This code yields the following video and plots of the simulation.

[Nonlinear Simulation (Google Photos)](https://photos.app.goo.gl/mzEL9quCqTYZ5SZXA)

And the plot,

![stateSpaceNonlinearSim](/SD403/Media/Images/StateSpaceNonlinearSim.png)

### Summary

Eventually the State Space model and observer will be implemented in C++ using the Eigen library, and will be integrated with the SPI_LED library and the interrupt code with the Look-up-table. Having an accurate estimate of the angle state over time is critical to the correct operation of the machine.

#### ECE Concept: State Space LQR Observer
