%% Digital RPM Sensor Algorithm
% Creating artificial test data

Fs = 1e3;
dt = 1/Fs;
t_end = 2;
t = 0:dt:t_end-dt;

% Data will be 16-bit unsigned ADC values (0-65535)
a = 10e3;
base = 30e3;
f = 5;
noise_std = 1e3;
data = a*sin(2*pi*f*t)+base;
data_noise = data + noise_std*randn(size(t));

figure; tiledlayout('vertical'); nexttile;
plot(t, data);
nexttile;
plot(t, data_noise);

%% Moving average envelope
avg = zeros(size(t));
trigger = zeros(size(t));
upper_thresh = 2;
lower_thresh = 1.5;
upper = zeros(size(t));
lower = zeros(size(t));
test = zeros(size(t));
thresh = 0;
W = 300;
for i = 1:numel(t)
    if i < W
        continue;
    end

    avg(i) = sum(abs(data_noise(i-W+1:i)))/W;
    
    upper(i) = (upper_thresh)*avg(i);
    lower(i) = (lower_thresh)*avg(i);
    test(i) = abs(data_noise(i)-avg(i));
    if abs(data_noise(i)-avg(i)) > upper_thresh*avg(i)
        thresh = 1;
    elseif abs(data_noise(i)-avg(i)) < lower_thresh*avg(i)
        thresh = -1;
    end

    if thresh == 1 
        trigger(i) = 1;
    elseif thresh == -1
        trigger(i) = -1;
    else
        trigger(i) = 0;
    end
end

hold on;
plot(t, avg);
plot(t,upper);
plot(t,lower);
plot(t,test);
legend('Data', 'avg', 'upper', 'lower', 'abs diff')
nexttile;
plot(t,trigger);
