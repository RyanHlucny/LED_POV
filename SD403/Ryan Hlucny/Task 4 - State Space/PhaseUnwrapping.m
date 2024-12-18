%% Data Fabrication
dt = 1e-3;
t_end = .25;
t = 0:dt:t_end-dt;
angle_delta = 2*pi/1e2;
noise_std = 2*pi/4e2;

% Creating synthetic angle data 
angle = 0:angle_delta:t_end/dt*angle_delta-angle_delta;
% Adding Gaussian noise
angle_noise = angle + randn(size(angle))*noise_std;

figure;
tiledlayout vertical;
nexttile;
plot(t, angle, '.'); grid on;
title('Synthetic angle data');
nexttile;
plot(t, angle_noise, '.'); grid on;
title('Synthetic angle data w/ noise')

% Wrap angle and shift to [-pi,pi): equivalent form as the sensor output
data_raw = mod(angle_noise, 2*pi) - pi;

nexttile;
plot(t, data_raw, '.'); grid on;
title('Raw synthetic data')

% Using matlab's unwrap function
unwrapped_data = unwrap(data_raw);
nexttile;
plot(t, unwrapped_data, '.'); grid on;
title('Unwrapped raw synthetic data')

%% Simulated real-time signal processing

% The approach to do the linear interpolation
% 1. Unwrap the data as it comes in
% 2. Use a sliding window of the previous N data points
% 3. Employ some technique to linearly interpolate (basic for 2 points or least squares for more)
% 4. Calculate the interpolated value
% 5. Rewrap the output value with modulo operation

figure;
tiledlayout vertical;
nexttile; hold on; grid on;
title('Simulated real-time DSP')

N = 2
% Filter parameters
d = [0];
a = d;
b = 1-d;

unwrapped = data_raw;
y = zeros(length(d), length(data_raw));

fs = 6660;
sample_period = 1/fs;
t_sample = 0:sample_period:t_end-sample_period;
t_sim = [t, t_sample];
samples = zeros(size(t_sample));

t_sensor_labels = repmat(["t_sensor"], size(t));
t_sample_labels = repmat(["t_sample"], size(t_sample));
t_labels = [t_sensor_labels, t_sample_labels];

% Sort the time and labels vectors to get an interrupt-like calling
% structure
[t_sim, I] = sort(t_sim);
t_labels = t_labels(I);
sensor_idx = 0;
sample_idx = 0;

for i = 1:length(t_sim)
    %% Handle new sensor data
    if t_labels(i) == "t_sensor"
        sensor_idx = sensor_idx + 1;
        % Unwrapping
        if sensor_idx < 2 
            continue
        end
        delta = unwrapped(sensor_idx) - unwrapped(sensor_idx-1);
        if delta >= pi
            while ~(delta < pi)
                unwrapped(sensor_idx) = unwrapped(sensor_idx) - 2*pi;
                delta = unwrapped(sensor_idx) - unwrapped(sensor_idx-1);
            end
        elseif delta <= -pi
            while ~(delta > -pi)
                unwrapped(sensor_idx) = unwrapped(sensor_idx) + 2*pi;
                delta = unwrapped(sensor_idx) - unwrapped(sensor_idx-1);
            end
        end
        % Plot current data point
        scatter(t(sensor_idx), unwrapped(sensor_idx), '.')
        
        % Apply IIR low-pass smoothing filter(s)
        for j = 1:length(d)
            y(j,sensor_idx) = a(j)*y(j,sensor_idx-1) + b(j)*unwrapped(sensor_idx);
        end
    end

    %% Handle new sample requisition

    if t_labels(i) == "t_sample"
        sample_idx = sample_idx + 1;

        % Linear Interpolation/extrapolation using output of filter
        if sensor_idx < 2
            continue;
        end
        now = t_sim(i);
        m = (y(1,sensor_idx)-y(1,sensor_idx-1))/(t(sensor_idx)-t(sensor_idx-1));
        samples(sample_idx) = m*(now-t(sensor_idx))+y(1,sensor_idx);
    end
    
end

plot(t, unwrapped)
scatter(t,y,'.')
plot(t, y)
scatter(t_sample, samples, 'k.')

nexttile;
plot(t,data_raw); hold on; grid on;
title('Rewrapped processed signal');
scatter(t,data_raw,'.')
scatter(t_sample,mod(samples+pi,2*pi)-pi,'.')
plot(t_sample,mod(samples+pi,2*pi)-pi)