dt = 1e-3;
t_end = .5;
t = 0:dt:t_end-dt;
deg_delta = 1;
noise_std = 1;

upsample_factor = 4;
t_upsampled = 0:dt/upsample_factor:t_end-dt/upsample_factor;

deg = 0:deg_delta:length(t)-1;
deg_noise = deg + randn(size(deg))*noise_std;

data = mod(deg_noise,360);

figure;
tiledlayout flow;
nexttile; grid on;
plot(t, deg_noise, '.')
nexttile; grid on;
plot(t, data, '.')
nexttile; grid on;
plot(t, data, '.')
hold on

lerp = zeros(1, length(data)*upsample_factor);
t_upsampled = zeros(size(lerp));

for i = 1:length(data)-1
    m = (data(i+1) - data(i)) / (t(i+1) - t(i))
    y = @(x) m*(x-t(i)) + data(i)
    for j = 1:upsample_factor
        t_desired = t(i+1)+dt/upsample_factor*j;
        % t_desired = t(i+1)+dt
        deg_est_raw = y(t_desired)
        if deg_est_raw < 0
            deg_est = deg_est_raw + 360
        elseif deg_est_raw >= 360
            deg_est = deg_est_raw - 360
        else
            deg_est = deg_est_raw
        end
        lerp(i*upsample_factor+j) = deg_est;
        t_upsampled(i*upsample_factor+j) = t_desired;
    end
    line([t(i) t_desired], [data(i), deg_est_raw],'LineStyle','--');
    plot(t_desired, deg_est, '*')
end

nexttile
plot(t_upsampled, lerp, '.')
