%% Segmentation of Nuclei Images
close all; clc; clearvars;

% Add path for SIFT
run('C:/vlfeat-0.9.20-bin/vlfeat-0.9.20/toolbox/vl_setup')

%% Read in nuclei information
nucImg1 = im2double(imread('DZ6079a_02A05 DAPI 10x.tif'));
nucImg2 = im2double(imread('DZ6079a_02A07 DAPI 10x.tif'));

images = {nucImg1, nucImg2};

%% Morphological Operators
for i = 2:2
    img = images{i};
    %figure; imshow(img); axis on

    % Canny Edge Detection
    [~, threshold] = edge(img, 'canny');
    factor = 1;
    BWs = edge(img,'canny', threshold * factor);
    %figure; imshow(BWs); title('canny edges')

    % Dilate the image
    se90 = strel('line', 3, 90);
    se0 = strel('line', 3, 0);

    BWsdil = imdilate(BWs, [se90 se0]); 
    %figure, imshow(BWsdil), title('dilated gradient mask');

    % Fill in the holes
    BWdfill = imfill(BWsdil, 'holes');
    %figure, imshow(BWdfill);
    %title('binary image with filled holes');

    % Smooth the object
    seD = strel('diamond',1);
    BWsmoothed = imopen(imerode(BWdfill,seD),seD);
    %figure, imshow(BWsmoothed), title('segmented image');

    % Label regions
    BWLabeled = bwlabel(BWsmoothed);
    figure, imagesc(BWLabeled), title('Morphological Operators'); colorbar
    
    % Draw equivalent circles
    cc = regionprops(BWsmoothed,'EquivDiameter', 'Centroid');
    equivDiameters = {cc.EquivDiameter};
    centroids = {cc.Centroid};
    
    figure; imshow(img); axis on; title('morphological connected regions');
    hold on;
    for i = 1:length(centroids)
        x = centroids{i}(1);
        y = centroids{i}(2);
        r = equivDiameters{i}/2;
        circle(x, y, r);
    end 
    
    maxNumNucs{i} = max(BWLabeled(:))
end
%% SIFT detector
SIFTPeakThresh = [0.04 0.04];
SIFTEdgeThresh = [15 15];

for nImage = 2:2 %length(images)
    
    warning off;

    % Detect SIFT keypoints for dog image
    img = images{nImage};
    if size(img,3) > 1
        imgGray = rgb2gray(img);
    else
        imgGray = img;
    end
    [frames, d] = vl_sift(single(imgGray), ...
        'PeakThresh', SIFTPeakThresh(nImage), ...
        'EdgeThresh', SIFTEdgeThresh(nImage));
    frames = frames';
    plotBlob(img, frames);
    set(gcf, 'Color', 'w');
    title('SIFT keypoints')
    disp(sprintf('%d SIFT keypoints', size(frames,1)));
    

end % nImage


%% SURF detector
isThresholding = true; 
%SURFNum = 2000;
multiplier = 0.66;
for nImage = 2:2 %length(images)
    
    warning off;

    % Detect SIFT keypoints for dog image
    img = images{nImage};
    if size(img,3) > 1
        imgGray = rgb2gray(img);
    else
        imgGray = img;
    end
    
    if isThresholding == true
        lvl = graythresh(img);
        imgGray = imbinarize(img, lvl);
        figure; imshow(imgGray, []);
        [L, Num] = bwlabel(imgGray);
        Num
    end
    
    SURFNum = round(Num * multiplier); %multiplier
    % Detect SURF keypoints
    points = detectSURFFeatures(imgGray);
    pointsSelected = points.selectStrongest(SURFNum);
    frames = zeros(SURFNum, 3);
    alpha = 1.3;
    for n = 1 : size(pointsSelected, 1)
        frames(n, 1 : 2) = pointsSelected(n).Location;
        frames(n, 3) = pointsSelected(n).Scale * alpha;
    end
    plotBlob(img, frames);
    set(gcf, 'Color', 'w');
    title('SURF keypoints');
    disp(sprintf('%d SURF keypoints', size(frames,1)));
    
    warning on;

end % nImage

%% Hough Transform Best Results by far
for i = 2:2
    img = images{i};
    sensitivity = 0.80;
    findNucleiSeg(img, sensitivity)
end

%% Observations
% 4 different methods: best results: Hough Transform, most accurate: 
% 1) Morphological Operators: 1957, but unable to clearly identify the
% number of nuclei, especially ones that are clumped together. 
% 2) SIFT detector: 657, lack of true positives
% 3) SURF detector: 6576, too many false positives, identify key detector
% in voids in betwen nuclei
% 4) Hough Transform: 1957, balance betweeen true positives and true
% negatives, also accurately identifies the number of nucleis clumped
% together. 