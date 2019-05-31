function [cellCount, nucleiCount] = getCellandNucleiCount(cellBoundaryImg, nucleiImg)
    %% Cell Boundary Segmentation Extraction
    cc = regionSegmentation(cellBoundaryImg);

    cellProps = regionprops(cc, 'Area','Eccentricity','Perimeter','EquivDiameter','MajorAxisLength','MinorAxisLength','Orientation', 'Centroid', 'PixelList', 'BoundingBox'); 
    cellProps2 = regionprops(cc, 'Centroid', 'PixelList');
    cellConvexImg = regionprops(cc, 'ConvexImage');
    
    %% Nuclei Segmentation
    % 1) use findNuclei to find centers - round to nearest integer
    % for each nuclei:
    % 2) find the nearest regions
    % 3) check if the nuclei centers lie in any regions
    % 4) update the nuclie count of that region.

    % nuclei segmentation
    sensitivity = 0.80; %increase sensivitiy finds more nuclei
    [centers, radii, metrics] = findNucleiSeg(nucleiImg,sensitivity);

    % define parameters to assign nuclei to region
    centers = round(centers);
    cellStruct = cellProps2;
    pixelLimit = 25;
    nucleiCount = zeros(size(cellStruct,1));
    nucleiSize = zeros(size(cellStruct,1));

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
        nucleiAssigned = false;
        for nNearestRegion = 1:length(nearestRegions)
            regionInd = nearestRegions{nNearestRegion};
            logical = ismember(centers(k,:), cellStruct(regionInd).PixelList, 'rows');
            if logical == 1
                %disp('xxx')
                %disp(regionInd)
                nucleiCount(regionInd) = nucleiCount(regionInd) + 1;
                nucleiSize(regionInd) = nucleiCount(regionInd) + radii(k)^2;
                nucleiAssigned = true; 
            end
        end
    end

    % Assign nucleiCount to new assignment
    tempStruct = cellProps;
    for k = 1:length(nucleiCount)
        tempStruct(k).nucleiCount = nucleiCount(k);
        tempStruct(k).nucleiSize = nucleiSize(k);
    end
    cellProps = tempStruct;

%     noNucInd = find([cellProps.nucleiCount] == 0);
%     noNucleiCellStruct = tempStruct(noNucInd); %region props without nuclei
% 
%     nucInd = find([cellProps.nucleiCount] > 0);
%     nucleiCellStruct = tempStruct(nucInd); %cell props containing nuclei
%     
    nucleiCounts = [cellProps.nucleiCount];
    nucleiCounts = nucleiCounts(nucleiCounts ~= 0);
    numofNuclei = unique(nucleiCounts);
    cellCounts = histc(nucleiCounts, numofNuclei);
    
    cellCount = [numofNuclei; cellCounts];
    nucleiCount = sum(nucleiCounts);
    
end
