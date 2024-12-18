%% Digital RPM Sensor Algorithm
% Creating artificial test data

Fs = 1e4;
dt = 1/Fs;
t_end = 10;
t = 0:dt:t_end-dt;

% Data will be 16-bit unsigned ADC values (0-65535)
a = 10e3;
base = 30e3;
f = 5;
noise_std = 1e3;
data = a*(sin(2*pi*f/2*t).^2-0.5*sin(4*pi*f/2*t).^2)+base;
data_noise = data + noise_std*randn(size(t));

figure; tiledlayout('vertical'); nexttile;
plot(t, data); title('Data without Noise'); xlabel('time');
nexttile;
plot(t, data_noise);

%% Moving average envelope
avg = zeros(size(t));
trigger = zeros(size(t));
f = zeros(size(t));
state = 0;
W = 2000;
rising_edge = 0;
prev_rising_edge = 0;
up_thresh = ones(size(t))*0.1*65535;
down_thresh = -ones(size(t))*0.01*65535;
for i = 1:numel(t)
    if i < 2
        avg(i) = data_noise(i);
        % continue;
    elseif i <= W
        avg(i) = avg(i-1)*(i-1)/i + data_noise(i)/i;
    else
        avg(i) = avg(i-1) + (data_noise(i)-data_noise(i-W))/W;
        % sum(data_noise(i-W+1:i))/W;
    end
    
    DC = data_noise(i) - avg(i);

    % State variable control
    if state == 0
        if DC > up_thresh(i)
            state = 1;
            rising_edge = t(i);
            % f calculation
            period_sec = rising_edge - prev_rising_edge;
            freq = 1/period_sec;
            f(i) = freq;
        end
    end
    if state == 1
        if DC < down_thresh(i)
            state = 0;
        end
    end

    % State machine output
    if state == 0
        trigger(i) = 0;
    elseif state == 1
        trigger(i) = 1;
    end

    prev_rising_edge = rising_edge;
end

hold on;
plot(t, avg); grid on; title('Signal with Noise and Moving Average'); xlabel('time');
nexttile;
DC_shift = data_noise-avg;
plot(t,DC_shift); grid on; title('DC Shifted Signal'); xlabel('time');
hold on;
plot(t, up_thresh); plot(t, down_thresh); legend('Signal','Up Thresh','Down Thresh');
nexttile;
plot(t,trigger); grid on; title('Trigger'); xlabel('time');
nexttile;
plot(t(f~=0),f(f~=0)); grid on; ylabel('Hz'); xlabel('time');
