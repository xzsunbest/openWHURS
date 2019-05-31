close all; clc; clear all;

warning('off','all');
img_names = {'DZ6079a_02A05', 'DZ6079a_02A08'};
load('Model');

cellCountMap = containers.Map();
nucleiCountMap = containers.Map();

for k = 1 : length(img_names)
    name = img_names{k};
    
    load(strcat('newFeatures3/', name, '.mat')); % Precomputed features
    
    % merged image
    mergedImg = im2double(imread(sprintf('images/merge %s 10x.tif', name)));
    cellBoundaryImg = im2double(imread(sprintf('images/%s Phalloidin Cy5 10x.tif', name)));
    nucleiImg = im2double(imread(sprintf('images/%s DAPI 10x.tif', name)));
    
    
    [cellCount, nucleiCount] = getCellandNucleiCount(cellBoundaryImg, nucleiImg);
    cellCountMap(name) = cellCount;
    nucleiCountMap(name) = nucleiCount;
    
    figure, imshow(mergedImg), title(name), hold on;
    
    
    
    Xk = cell2mat(struct2cell(rmfield(cellProps, ...
        {'Area', 'Centroid', 'BoundingBox', 'MajorAxisLength', 'MinorAxisLength', 'Eccentricity', ...
         'Orientation', 'EquivDiameter', 'PixelList', 'Perimeter', 'label', ...
         'nearbyCellSizeVsDist','distFromBD'})))';
    Xk(isnan(Xk)) = 0;
     
    Yk = zeros(length(cellProps), 1);
    for l = 1 : length(cellProps)
        cent = cellProps(l).Centroid;
        
        
        if ~strcmp(cellProps(l).label, 'IC') && ~strcmp(cellProps(l).label, 'C')
            pred = predict(SVMMdl, Xk(l, :)); % Prediction
                        
            if pred == 1
                text(cent(1), cent(2), 'BD', 'Color', 'c', 'FontSize', 6)
            elseif pred == 2
                text(cent(1), cent(2), 'CV', 'Color', 'g', 'FontSize', 6)
            else
                text(cent(1), cent(2), 'PV', 'Color', 'm', 'FontSize', 6)
            end
        end
    end  
end

for k = 1 : length(img_names)
    name = img_names{k};
    cC = cellCountMap(name);
    disp(strcat('Image name: ', name));
    for i = 1:3
        disp(sprintf('Number of %d nuclei cells: %d', i, cC(2, i)));
    end
    disp(sprintf('Number of nucleis: %d', nucleiCountMap(name)));
end

