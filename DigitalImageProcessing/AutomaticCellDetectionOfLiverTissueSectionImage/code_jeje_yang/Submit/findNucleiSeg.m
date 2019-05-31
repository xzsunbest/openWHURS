function [centers, radii, metric] = findNucleiSeg(img, sensitivity)
    % This function takes in a grayscale nuclie image and return the
    % centers, radii, and metric of the segmented nuclie using Hough 
    % Transform circle transform
    showPlots = false;
    delta = 0.05; %internal parameter to be tuned to reduce boundary edges
    lvl = graythresh(img) - delta;
    
    imgBW = imbinarize(img, lvl);

    BWs = edge(imgBW, 'canny');
    % Dilate the image
    se90 = strel('line', 3, 90);
    se0 = strel('line', 3, 0);
    BWsdil = imdilate(BWs, [se90 se0]);
    
    % Automatically detect the radius range of cells
    cc = regionprops(BWsdil,'EquivDiameter');
    equivDiameters = {cc.EquivDiameter};
    nucleiRadii = cell2mat(equivDiameters) ./ 2;
    Rmean = mean(nucleiRadii);
    Rstd = std(nucleiRadii);
    
    Rmin = floor(prctile(nucleiRadii, 25));
    Rmax = ceil(prctile(nucleiRadii, 99));

    % Find circles using Hough transform
    %Rmin = 5;     %Rmax = 20;
    [centers, radii, metric] = imfindcircles(BWsdil, [Rmin Rmax], 'Sensitivity', sensitivity);
       
    if showPlots
        figure, imshow(imgBW); title('Threshold image'); axis on;
        figure, imshow(BWs); title('Edge Detection'); axis on;
        figure, imshow(BWsdil), title('Dilated Edge'); axis on;
    end
    
    figure, imshow(img), hold on;
    plot(centers(:, 1), centers(:, 2), 'xr',  'MarkerSize', 2, 'LineWidth', 1);
    viscircles(centers, radii, 'EdgeColor','b', 'LineWidth', 1);
    title('Hough Circle Transform'), axis on
end