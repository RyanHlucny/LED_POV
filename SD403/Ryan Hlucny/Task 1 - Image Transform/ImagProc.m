%% Image importing
addpath("Images\");
imgpath = 'NDSU-football.jpg';
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

%% Polar Plotting
num_LEDs_right = 43; % 43
num_LEDs_left = 43; % 41
ledSpacing = 0.2734; % Distance from center to center of LEDs in inches
r_start_right = 0.05; % Distance from center of rotation to the first LED on right blade
r_start_left = r_start_right+ledSpacing/2; % Distance from center of rotation to the first LED on left blade
r_max_right = num_LEDs_right*ledSpacing;
r_max_left = num_LEDs_left*ledSpacing;
r = sort([r_start_right:ledSpacing:r_max_right, r_start_left:ledSpacing:r_max_left]);
% px_const = dr;
% dq = px_const/r_max; % Use this for "square-like spacing"
dq = 2*pi/275;
q = dq:dq:2*pi;

r_max = max(r_max_right,r_max_left)
scalex = dimsC(2)/r_max/2;
scaley = dimsC(1)/r_max/2;
h = dimsC(1); w = dimsC(2);

x = zeros(length(r),length(q));
y = zeros(length(r),length(q));
c = zeros(length(r),length(q),3);

% Creation of LUT
idx = 0;
for j = 1:length(q), for i = 1:length(r)
    theta = -q(j)+pi/2;
    x(i,j) = r(i)*cos(theta); % inches
    y(i,j) = r(i)*sin(theta); % inches
    x_transform = max(round(w/2+x(i,j)*scalex),1);
    y_transform = max(round(h/2-y(i,j)*scaley),1);
    rgb = C(y_transform,x_transform,:);
    c(i,j,:) = rgb;
    
end, end

% Plotting
figure(2); clf(2); subplot(211);
x_prime = reshape(x,[size(x,1)*size(x,2) 1]);
y_prime = reshape(y,[size(y,1)*size(y,2) 1]);
c_prime = reshape(c,[size(c,1)*size(c,2) 3]);
scatter(x_prime,y_prime,10,double(c_prime)/255,"filled"); grid on; axis equal;
xlabel('x (inches)'); ylabel('y (inches)');
xlim([-13 13]); ylim([-13 13]);


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

