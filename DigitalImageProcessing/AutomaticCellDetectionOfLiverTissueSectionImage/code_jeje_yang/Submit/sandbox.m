%%
for k = 1:size(centers,1)
    
    % find the nearest regions
    count = 1;
    nearestRegions = {};
    for j = 1:size(cellStruct,1)
        distance = norm(centers(k,:) - cellStruct(j).Centroid);
        if distance < pixelLimit
            nearestRegions{count} = j;
            count = count + 1;
        end
    end
    
    % check if the nuclei centers lie in any of the nearest regions
    for nNearestRegion = 1:length(nearestRegions)
        regionInd = nearestRegions{nNearestRegion};
        logical = ismember(centers(k,:), cellStruct(regionInd).PixelList, 'rows');
        if logical ~= 0
            nucleiCount(regionInd) = nucleiCount(regionInd) + 1;
        end
    end        
end

%%
clearvars, close all, clc;
filename = 'DZ6081a_02A05';

cellBoundaryImg = im2double(imread(strcat(filename, ' Phalloidin Cy5 10x.tif')));
nucleiImg = im2double(imread(strcat(filename, ' DAPI 10x.tif')));

[cellCount, nucleiCount] = getCellandNucleiCount(cellBoundaryImg, nucleiImg)