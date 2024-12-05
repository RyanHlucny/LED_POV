
% Fs = 1e3;
% dt = 1/Fs;
% T_end = 3;
% t = 0:dt:T_end-dt;
% 
% f_target = 10; % hz
% A = 10e3;
% DC = 30e3;
% data = A*sin(2*pi*f_target*t) + DC;
% noise_std = 1e3;
% data_noise = data + noise_std.*randn(size(data));

t = data(:,1)/1e6;
data_noise = data(:,2);

figure; tiledlayout('vertical'); nexttile;
% plot(t, data); grid on; nexttile;
plot(t, data_noise); grid on;

W = 500;
avg = zeros(size(t));
trigger = zeros(size(t));
filtered_data = zeros(size(t));
DC_blocked_data = zeros(size(t));
f_calculated = zeros(size(t));
t_last = 0;

upper_thresh = 0.05*65535;
lower_thresh = 0.01*65535;
upper_env = lower_thresh*ones(size(t));
lower_env = -lower_thresh*ones(size(t));
state = 0;

% Data LPF
a = 0.75;
b = 1-a;

for i = 1:length(t)
    
    if (i < 2)
        avg(i) = data_noise(i);
    elseif (i < W+1)
        filtered_data(i) = a*filtered_data(i-1) + b*data_noise(i);
        avg(i) = avg(i-1) * (i-1)/i + data_noise(i)/i;
    else
        filtered_data(i) = a*filtered_data(i-1) + b*data_noise(i);
        avg(i) = avg(i-1) + (data_noise(i) - avg(i-W))/W;
    end

    dc_shifted = filtered_data(i) - avg(i);
    DC_blocked_data(i) = dc_shifted;
    
    if (dc_shifted > lower_thresh & state == 0)
        state = 1;
        f_calculated(i) = 1/(t(i) - t_last);
        t_last = t(i);
    end
    if (dc_shifted < -lower_thresh & state == 1)
        state = 0;
    end
    
    if (state == 1)
        trigger(i) = 1;
    elseif (state == 0)
        trigger(i) = 0;
    end
end

hold on; plot(t, avg); nexttile;
hold on; plot(t, filtered_data); plot(t, avg); grid on; nexttile;
hold on; plot(t, DC_blocked_data); grid on;
plot(t,upper_env); plot(t,lower_env); nexttile;
plot(t, trigger); grid on; nexttile;
plot(t(f_calculated~=0), f_calculated(f_calculated~=0)); grid on;
hold on; plot(t(f_calculated~=0), f_calculated(f_calculated~=0),'o');
