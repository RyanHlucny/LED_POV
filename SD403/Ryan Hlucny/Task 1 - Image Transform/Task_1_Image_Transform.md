
# HW04 - Task 1 - Image Transform

## Ryan Hlucny

### Task Overview

My first task includes creating a Look-up-table (LUT) which encodes the static image matrix data (pixel location and R, G, B values) into a new matrix which is indexed by a given angle $\theta$ and a radius $r$. This tasks includes the 4 following subtasks:

- Image pre-processing
- Pixel Transformation
- Twist Compensation (to counteract "swirl" effect)
- Brightness compensation

### Image pre-processing

Before I am able to use an image, it must first be converted into a usable file format. MATLAB has multiple different image colortypes (Indexed, truecolor, grayscale, etc). My matlab code is able to accept an image file which uses any of these color types. This is the relevant block of code which handles that:

```Matlab
%% Image importing
addpath("Images\");
imgpath = 'dahlia.jpg';
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
```

I have imported a directory of images for testing, which is included in this Github Repository within this folder.

Then the image is (optionally) reduced and center square cropped so that there is no stretching on the original image when displayed on the LED Spinner. The relevant code is here:

```Matlab
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

```

This code produces the following plots. Note that I am not reducing the image at all as the reduction factor is set to 1 in the code above.

![ndsu-football-preprocessing](/SD403/Media/Images/ndsu-football-preprocessing.png)

### Pixel Transformation

The general approach to the pixel transformation is shifting the origin from the corner of the image to the center and then constructing a new matrix $c(r,\theta)$ which returns a color vector of the form 

```math
\begin{bmatrix} 
R\\ 
G\\ 
B 
\end{bmatrix}
```

which is our look-up-table.

The thing that makes this LUT useful is the fact that the microcontroller won't have to do any of these transformation calculations on-board, but all of it can be done before-hand in pre-processing.

Since LED placement is bound to be imperfect and we may have a different number of LEDs on each blade, I made the code capable of producing LUTs for setups with any number of LEDs on either blade and given an led spacing and starting offset from the center. These are constants we'll need to measure and determine on our device once it is fully assembled to produce the most accurate image.

The algorithm is implemented in Matlab as follows

```Matlab
%% Polar Plotting and LUT Construction
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

figure(2); clf(2); tiledlayout('vertical');

% Variable parameters
% Represents a variable amount of 'twist' to predistort the image to help
% compensate for time lag in leds changing color. A value of 1 represents
% one full rotation at the outer ring of pixels. Set to 0 to disable
twist_const = 0.05;

[Q_twist,R] = meshgrid(q,r/r_max);
[m1,m2] = meshgrid(q,linspace(0,twist_const*2*pi,length(r)))
nexttile;
plot(R.*cos(Q_twist-m2),R.*sin(Q_twist-m2)); grid on; axis equal;
xlim([-1 1]); ylim([-1 1]); title("Twist Visualization");
subtitle(sprintf('Twist Factor = %f',twist_const));
% Represents the relative brightness of the center pixel compared
% to the pixels at the outer edge. Set to '1' to disable
relative_brightness_const = 0.75;

[Q,R] = meshgrid(q,r/r_max);
b = relative_brightness_const;
brightness_comp = (1-b)*sqrt((R.*cos(Q)).^2+(R.*sin(Q)).^2)+b;
% Plot brightness compensation surface (Z is the relative brightness)
nexttile;
surf(R.*cos(Q),R.*sin(Q),brightness_comp,'EdgeAlpha',0.25); axis equal; zlim([0,1]);
colorbar; colormap winter; title("Brightness Height Map");
subtitle(sprintf('Relative Brightness Compensation = %f',relative_brightness_const));

% Construction of LUT
idx = 0;
for j = 1:length(q), for i = 1:length(r)
    theta = -q(j)+pi/2;
    x(i,j) = r(i)*cos(theta); % inches
    y(i,j) = r(i)*sin(theta); % inches

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
    c(i,j,:) = rgb * brightness_comp(i,j);

end, end
```

You may notice there are a couple additional compensation factors at play here. First is the twist compensation.

### Twist Compensation

When we drive the LEDs, we send a stack of color 'commands' to the LEDs, which is essentially a fancy sequence of bits describing the colors of all the LEDs in that strip. We will explore more about this in the next section, "Task 2 - LED Control". Each LED pops the top-most color command on the stack, removing it and using it for itself, and sends the rest of the command stack to the subsequent LEDs. Since it takes time for the controller to take in the data and pass it along, this process has an inherent time delay. Since the data sheet makes no mention of this time delay, we will have to determine this experimentally.

Due to the time delay, as the device spins, there will appear to be a "twist" or "swirl" effect in the image. This problem is exacerbated with higher rotation speeds, but it is there, to some degree, in all cases. This is because as the data propogates from one LED to the next, by the time each subsequent LED gets its command, the LED has already moved from where it was at the start of the command. Therefore, the LEDs will be off by some $\Delta\theta$, which is a function of rotational speed, radius, and time.

To compensate for the twist dynamically, it would require the microcontroller to recompute the LUT each time, which is not feasible. Therefore, we are generating a single LUT with a preset twist_const based on the expected rotation speed.

The twist compensation is implemented as a constant `twist_const`, which describes the number of turns the outer ring of pixels will turn from the default image. This constant can be negative or positive, which applies a "swirl" effect to the image. This can be thought of as a "pre-distortion" to the image to compensate for the natural swirl distortion which occurs in the image.

The Matlab script I wrote generates a twist visualization plot to visualize the twist being applied to the image.

![twist_visual](/SD403/Media/Images/twist_visual.png)

### Brightness Compensation

Another factor that needs to be compensated is the brightness. As the device spins, at each $\Delta\theta$, the LEDs will get a new color command. The outer ring of LEDs are physically moving faster, which means the light from each LED is less concentrated in one spot in each $\Delta\theta$ interval.

Therefore a brightness compensation is required to equalize the brightness from the center of the image to the outside. The speed of each pixel is linearly proportional to radius, that is $$v \propto r$$ Hence the brightness compensation is implemented as a conical surface. The code above also displays a brightness height map visualization

![brightness-map](/SD403/Media/Images/brightness-map.png)

The relative brightness compensation of 0.75 means the center pixels are 75% the brightness of the outside pixels.

The resulting image, after applying the twist and brightness compensation (compensation factors enlarged for the sake of viewing) would look like the following

https://github.com/RyanHlucny/LED_POV/assets/31110301/453112db-85e5-4b11-892b-0687b9dbed2b

### Summary

The Matlab code is included in this directory so you can take a look at it and play with it as you wish. The output of the program that will be used in the Pico microcontroller is the $c(r,\theta)$ matrix, which is a matrix describing the RGB color values of each pixel as an 8-bit integer.

#### ECE Concept: Image Transform (Image Processing / Analysis)
