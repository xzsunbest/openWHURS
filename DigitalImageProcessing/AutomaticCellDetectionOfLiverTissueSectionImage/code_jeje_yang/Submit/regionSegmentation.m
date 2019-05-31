function [cc] = regionSegmentation(cellBoundaryImg)
% This function takes in an cell boundary iamge and returns an connected
% region labels

    %% Feature Extraction
    isSmoothed = true;
    showPlots = false;
  
    % Canny Edge Detection
    [~, threshold] = edge(cellBoundaryImg, 'canny');
    factor = 1;
    BWs = edge(cellBoundaryImg,'canny', threshold * factor);

    % Dilate the image
    se90 = strel('line', 2, 90);
    se0 = strel('line', 2, 0);
    BWsdil = imdilate(BWs, [se90 se0]); 

    % Negate the complement
    BWsdil = imcomplement(BWsdil);

    % Fill in the holes
    BWdfill = imfill(BWsdil, 'holes');

    % Smooth the Object
    if isSmoothed == true
        seD = strel('diamond',1);
        BWsmoothed = imopen(imerode(BWdfill,seD),seD);
        
    else
        BWsmoothed = BWdfill;
    end

    % Label regions
    BWLabeled = bwlabel(BWsmoothed);
    
    % Region Properties of the labeled image
    cc = bwconncomp(BWsmoothed);
    
    % Plot
    if showPlots
        figure; imshow(cellBoundaryImg), title('original image');
        figure; imshow(BWs); title('canny edges');
        figure, imshow(BWsdil), title('dilated gradient mask');
        figure, imshow(BWdfill); title('binary image with filled holes');
        figure, imshow(BWsmoothed), title('smoothed image'); 
    end
    figure, imagesc(BWLabeled), title('labeled image'); colorbar

end