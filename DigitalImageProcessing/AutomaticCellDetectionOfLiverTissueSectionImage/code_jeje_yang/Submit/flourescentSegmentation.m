%% Lab Space color segmentation of Cell Tissue
close all; clc; clearvars;

%% Read in set of images
filename = 'DZ6079a_02A05';

cellBoundaryImg = im2double(imread(strcat(filename, ' Phalloidin Cy5 10x.tif')));
nucleiImg = im2double(imread(strcat(filename, ' DAPI 10x.tif')));
flourescentImg = im2double(imread(strcat(filename, ' DsRed 10x.tif')));
overlayImg = im2double(imread(['merge', ' ', filename, ' 10x.tif']));
imgSet = {cellBoundaryImg, nucleiImg, flourescentImg, overlayImg};

for i = 1:numel(imgSet)
    figure
    imshow(imgSet{i}), axis on
end

%% Lab Space
% Calculate Sample Colors in L*a*b* Color Space for Each Region
fabric = overlayImg;
figure, imshow(fabric), title('fabric');

nColors = 3;
sample_regions = false([size(fabric,1) size(fabric,2) nColors]);

for count = 1:nColors
  sample_regions(:,:,count) = roipoly(fabric);
end

imshow(sample_regions(:,:,2)),title('sample region for red');

%% Convert your fabric RGB image into an L*a*b* image using rgb2lab .
lab_fabric = rgb2lab(fabric);

a = lab_fabric(:,:,2);
b = lab_fabric(:,:,3);
color_markers = zeros([nColors, 2]);

for count = 1:nColors
  color_markers(count,1) = mean2(a(sample_regions(:,:,count)));
  color_markers(count,2) = mean2(b(sample_regions(:,:,count)));
end

fprintf('[%0.3f,%0.3f] \n',color_markers(2,1),color_markers(2,2));

%% Classify Each Pixel Using the Nearest Neighbor Rule
% Create an array that contains your color labels, i.e., 0 = background, 1 = red, 2 = white
color_labels = 0:nColors-1;
a = double(a);
b = double(b);

distance = zeros([size(a), nColors]);
for count = 1:nColors
  distance(:,:,count) = ( (a - color_markers(count,1)).^2 + ...
                      (b - color_markers(count,2)).^2 ).^0.5;
end

[~, label] = min(distance,[],3);
label = color_labels(label);
clear distance;

%% label images
rgb_label = repmat(label,[1 1 3]);
segmented_images = zeros([size(fabric), nColors],'uint8');

for count = 1:nColors
  color = fabric;
  color(rgb_label ~= color_labels(count)) = 0;
  figure; imshow(color)
  %segmented_images(:,:,:,count) = color;
end

%figure;
%imshow(segmented_images(:,:,:,3), []), title('red objects');

