
# HW04 - Tasks 1 - Image Transform

## Ryan Hlucny

### Task Overview

My first task includes creating a Look-up-table (LUT) which encodes the static image matrix data (pixel location and R, G, B values) into a new matrix which is indexed by a given angle $\theta$ and a radius $\rho$. This tasks includes the 4 following subtasks:

- Image pre-processing
- Pixel Transformation
- Brightness compensation
- Lag Compensation (to prevent "swirl" effect)

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

