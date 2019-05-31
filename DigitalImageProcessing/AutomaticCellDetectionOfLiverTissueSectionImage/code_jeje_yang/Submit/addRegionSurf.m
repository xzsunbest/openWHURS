close all; clc; clear all;

% img_names = {'DZ6079a_02A05', 'DZ6079a_02A06', 'DZ6079a_02A07', ...
%              'DZ6079a_02A08', 'DZ6079a_02A09', 'DZ6079a_02A10', 'DZ6079a_02A11', ...
%              'DZ6079a_02A12', 'DZ6080a_02B01', 'DZ6080a_02B02', 'DZ6080a_02B03'};
img_names = {'DZ6079a_02A07'};
         
se = strel('square', 30);        
         
for i = 1 : length(img_names)
    i
    name = img_names{i};
    load(strcat('newFeatures3/', name, '.mat'));
    img = im2double(imread(sprintf('mergedImages/merge %s 10x.tif', name)));
    img_gray = rgb2gray(img);
    
    %imshow(img_gray);
    %newProps = cellProps;
    
    for j = 1 : length(cellProps)
        j
        c = cellProps(j);
        
                
        
        if strcmp(c.label, 'CV')
            
        mask = zeros(size(img_gray));
        for row = 1 : length(c.PixelList)
            pix = c.PixelList(row, :);
            mask(pix(2), pix(1)) = 1;
        end        
        newmask = imdilate(mask, se);
        b = regionprops(newmask, 'BoundingBox', 'Area');
        %if ~isfield(c, 'SurfCount')
            regimg = imcrop(img_gray, b.BoundingBox) .* imcrop(newmask, b.BoundingBox);
            surfFeatures = detectSURFFeatures(regimg);
            cellProps(j).SurfCount = length(surfFeatures);
            cellProps(j).SurfDensity = length(surfFeatures) / sum(sum(newmask - mask));
        %end
        
        boundaryimg = imcrop(img_gray, b.BoundingBox) .* imcrop(newmask - mask, b.BoundingBox);
        bsurfFeatures = detectSURFFeatures(boundaryimg);
        cellProps(j).BoundSurfCount = length(bsurfFeatures);
        cellProps(j).BoundSurfDensity = length(bsurfFeatures) / sum(sum(newmask - mask));
        
        if length(cellProps(j).nearbyCellSizeVsDist) == 0
            nbSize = zeros(20, 2);
        else
            nbSize = cellProps(j).nearbyCellSizeVsDist;
        end
        for lev = 1 : 20
            cellProps(j).(sprintf('nbSize%d', lev)) = nbSize(lev, 2);
        end  
        end
        
              
    end
    
    %save(sprintf('newFeatures3/%s.mat', name), 'cellProps', 'cellConvexImg');
end