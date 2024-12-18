%% Image importing
addpath("Images\");
imgpath = 'snowy-mountain-peak-starry-galaxy.jpg';
info = imfinfo(imgpath)
% Handle indexed and RGB images
if (info.ColorType == "indexed")
    % Indexed color type
    [A, Acmap] = imread(imgpath);
    A = ind2rgb(A, Acmap);
    A = uint8(A*255);
elseif(info.ColorType == "truecolor")
    % RGB color type
    A = imread(imgpath);
elseif(info.ColorType == 'grayscale')
    % Any other color type
    A = imread(imgpath);
    A = repmat(A,1,1,3);
end
figure(1); clf(1); subplot(311);
image(A); axis image; title('Original');
subplot(312);
dimsA = size(A); dimsA = dimsA(1:2);
reductionFactor = 1;
dimsB = round(dimsA/reductionFactor); 
B = imresize(A,dimsB);
image(B); axis image; title('Reduced');
subplot(313);
r = centerCropWindow2d(dimsB,[min(dimsB) min(dimsB)]);
C = imcrop(B,r); dimsC = size(C);
image(C); axis image; title('Center Square Crop');
rectangle('Position',[0 0 dimsC(1) dimsC(2)],'Curvature',[1 1]);

%% Polar Plotting and LUT Construction
num_LEDs_right = 53;
num_LEDs_left = 53;
ledSpacing = 0.2734; % Distance from center to center of LEDs in inches
r_start_right = 0.05; % Distance from center of rotation to the first LED on right blade
r_start_left = r_start_right+ledSpacing/2; % Distance from center of rotation to the first LED on left blade
r_max_right = num_LEDs_right*ledSpacing;
r_max_left = num_LEDs_left*ledSpacing;
r_right = r_start_right:ledSpacing:r_max_right;
r_left = r_start_left:ledSpacing:r_max_left;
r = sort([r_right, r_left]);
% px_const = dr;
dq = ledSpacing/r_max_right; % Use this for "square-like spacing"
% dq = 2*pi/275;
% dq = 0.0106
% dq = 0.00353868996500354
q = dq:dq:2*pi;

r_max = max(r_max_right,r_max_left)
scalex = dimsC(2)/r_max;
scaley = dimsC(1)/r_max;
h = dimsC(1); w = dimsC(2);

figure(2); clf(2); tiledlayout('vertical');

% Variable parameters
% Represents a variable amount of 'twist' to predistort the image to help
% compensate for time lag in leds changing color. A value of 1 represents
% one full rotation at the outer ring of pixels. Set to 0 to disable
twist_const = 0; %0.075;

[Q_twist,R] = meshgrid(q,r/r_max);
[m1,m2] = meshgrid(q,linspace(0,twist_const*2*pi,length(r)));
nexttile;
plot(R.*cos(Q_twist-m2),R.*sin(Q_twist-m2)); grid on; axis equal;
xlim([-1 1]); ylim([-1 1]); title("Twist Visualization");
subtitle(sprintf('Twist Factor = %f',twist_const));
% Represents the relative brightness of the center pixel compared
% to the pixels at the outer edge. Set to '1' to disable
relative_brightness_const = 1;%0.5;

[Q,R] = meshgrid(q,r/r_max);
b = relative_brightness_const;
brightness_comp = (1-b)*sqrt((R.*cos(Q)).^2+(R.*sin(Q)).^2)+b;
% Plot brightness compensation surface (Z is the relative brightness)
nexttile;
surf(R.*cos(Q),R.*sin(Q),brightness_comp,'EdgeAlpha',0.25); axis equal; zlim([0,1]);
colorbar; colormap winter; title("Brightness Height Map");
subtitle(sprintf('Relative Brightness Compensation = %f',relative_brightness_const));

% Let right side be long side and left side be short side for now
LUT_long = zeros(1, length(r_right)*length(q)*3);
LUTidx = 1;
x = zeros(length(r_right),length(q));
y = zeros(length(r_right),length(q));
c = uint8(zeros(length(r_right),length(q),3));

% Construction of LUT for Long blade
for j = 1:length(q), for i = 1:length(r_right)
    theta = -q(j)-pi/2;
    x(i,j) = r_right(i)*cos(theta); % inches
    y(i,j) = r_right(i)*sin(theta); % inches

    % Transforming from corner-origin cartesian coordinates to
    % centered polar coordinates
    x_transform = max(round(w/2+x(i,j)*scalex),1);
    y_transform = max(round(h/2-y(i,j)*scaley),1);
    
    % Produce a twist proportional to radius s/r = theta
    theta_twist = theta+twist_const*r(i)/r_max*2*pi;
    x_twist = r(i)*cos(theta_twist);
    y_twist = r(i)*sin(theta_twist);
    x_transform_twist = max(round(w/2+x_twist*scalex),1);
    y_transform_twist = max(round(h/2-y_twist*scaley),1);
    
    rgb = C(y_transform_twist,x_transform_twist,:);
    
    % Brightness Compensation
    c(i,j,:) = uint8(rgb * brightness_comp(i,j));
    % Reformat data into flat array and in RBG color ordering
    LUT_long(LUTidx:LUTidx+2) = [rgb(1) rgb(3) rgb(2)] * brightness_comp(i,j);
    LUTidx = LUTidx + 3;
end, end

% Plotting
figure(3); clf(3); tiledlayout('horizontal'); nexttile;
x_prime = reshape(x,[size(x,1)*size(x,2) 1]);
y_prime = reshape(y,[size(y,1)*size(y,2) 1]);
c_prime = reshape(c,[size(c,1)*size(c,2) 3]);
scatter(x_prime,y_prime,10,double(c_prime)/255,"filled"); grid on; axis equal;
xlabel('x (inches)'); ylabel('y (inches)');
xlim([-13 13]); ylim([-13 13]); title("Rendered LUT Long");

LUT_short = zeros(1, length(r_left)*length(q)*3);
LUTidx = 1;
x = zeros(length(r_left),length(q));
y = zeros(length(r_left),length(q));
c = uint8(zeros(length(r_left),length(q),3));

% Construction of LUT for Short blade
for j = 1:length(q), for i = 1:length(r_left)
    theta = -q(j)+pi/2;
    x(i,j) = r_left(i)*cos(theta); % inches
    y(i,j) = r_left(i)*sin(theta); % inches

    % Transforming from corner-origin cartesian coordinates to
    % centered polar coordinates
    x_transform = max(round(w/2+x(i,j)*scalex),1);
    y_transform = max(round(h/2-y(i,j)*scaley),1);
    
    % Produce a twist proportional to radius s/r = theta
    theta_twist = theta+twist_const*r(i)/r_max*2*pi;
    x_twist = r(i)*cos(theta_twist);
    y_twist = r(i)*sin(theta_twist);
    x_transform_twist = max(round(w/2+x_twist*scalex),1);
    y_transform_twist = max(round(h/2-y_twist*scaley),1);
    
    rgb = C(y_transform_twist,x_transform_twist,:);
    
    % Brightness Compensation
    c(i,j,:) = uint8(rgb * brightness_comp(i,j));
    % Reformat data into flat array and in RBG color ordering
    LUT_short(LUTidx:LUTidx+2) = [rgb(1) rgb(3) rgb(2)] * brightness_comp(i,j);
    LUTidx = LUTidx + 3;
end, end

nexttile;
x_prime = reshape(x,[size(x,1)*size(x,2) 1]);
y_prime = reshape(y,[size(y,1)*size(y,2) 1]);
c_prime = reshape(c,[size(c,1)*size(c,2) 3]);
scatter(x_prime,y_prime,10,double(c_prime)/255,"filled"); grid on; axis equal;
xlabel('x (inches)'); ylabel('y (inches)');
xlim([-13 13]); ylim([-13 13]); title("Rendered LUT Short");


% Generating the LUT file as a csv file
[pathstr,name,ext] = fileparts(info.Filename);
writematrix(LUT_long,sprintf("LUT_long-%s.csv", name));
writematrix(LUT_short,sprintf("LUT_short-%s.csv", name));

%% LUT_async creation
LUT_async_long = zeros(1, length(LUT_long)+length(q)*(4+num_LEDs_right+floor((num_LEDs_right+15)/16)));
LUT_async_short = zeros(1, length(LUT_short)+length(q)*(4+num_LEDs_left+floor((num_LEDs_left+15)/16)));

jc = 1;
ic = 1;
for i = 1:length(q)
    for d = 1:4
        LUT_async_long(jc) = 0;
        jc = jc+1;
    end
    for d = 1:num_LEDs_right
        LUT_async_long(jc) = 255; jc = jc+1;
        LUT_async_long(jc:jc+2) = LUT_long(ic:ic+2);
        jc = jc+3; ic = ic+3;
    end
    for d = 1:floor((num_LEDs_right+15)/16)
        LUT_async_long(jc) = 255; jc = jc+1;
    end
end

ic = 1;
jc = 1;
for i = 1:length(q)
    for d = 1:4
        LUT_async_short(jc) = 0;
        jc = jc+1;
    end
    for d = 1:num_LEDs_left
        LUT_async_short(jc) = 255; jc = jc+1;
        LUT_async_short(jc:jc+2) = LUT_short(ic:ic+2);
        jc = jc+3; ic = ic+3;
    end
    for d = 1:floor((num_LEDs_left+15)/16)
        LUT_async_short(jc) = 255; jc = jc+1;
    end
end

writematrix(LUT_async_long,sprintf("LUT_async_long-%s.csv", name));
writematrix(LUT_async_short,sprintf("LUT_async_short-%s.csv", name));

%% Simulation
% figure(4); clf(4); hold on; grid on; axis equal;
% xlim([-13 13]); ylim([-13 13]);
% % q_est = .5; % radians
% % q_idx = max(round(q_est/(2*pi)*length(q)),1);
% for q_est = 0:0.01:2*pi
%     clf(4); hold on; grid on; axis equal;
%     xlim([-13 13]); ylim([-13 13]);
%     q_idx = max(round(q_est/(2*pi)*length(q)),1);
%     for Q = q_idx, for R = 1:length(r);
%        scatter(x(R,Q),y(R,Q),10,double([c(R,Q,1) c(R,Q,2) c(R,Q,3)])/255,"filled");
%     end, end
% % pause(0.001);
% end 
% axis equal;

