%% Segmentation of Cell Tissue
close all; clc; clearvars;

files = dir('newFeatures3/*.mat');

for k = 1 : length(files)
    k
    load(strcat('newFeatures3/', files(k).name))
    
    bd_ind = find(strcmp({cellProps.label}, 'BD') == 1)
    s = {};
    for ind = 1 : length(bd_ind)
        s{ind} = {bd_ind(ind), 'BD'};
    end
    
    cellProps = addVesselCellSizeDistFeature(s, cellProps);
    
    for j = 1 : length(s)
        ind = s{j}{1}
        if length(cellProps(ind).nearbyCellSizeVsDist) == 0
            nbSize = zeros(20, 2);
        else
            nbSize = cellProps(ind).nearbyCellSizeVsDist;
        end
        for lev = 1 : 20
            cellProps(ind).(sprintf('nbSize%d', lev)) = nbSize(lev, 2);
        end        
    end
    
    save(strcat('newFeatures/', files(k).name), 'cellProps', 'cellConvexImg');
    
end

%% Read in set of images
% filename = 'DZ6081a_02A08';
% 
% cellBoundaryImg = im2double(imread(strcat(filename, ' Phalloidin Cy5 10x.tif')));
% nucleiImg = im2double(imread(strcat(filename, ' DAPI 10x.tif')));
% flourescentImg = im2double(imread(strcat(filename, ' DsRed 10x.tif')));
% overlayImg = im2double(imread(['merge', ' ', filename, ' 10x.tif']));
% imgSet = {cellBoundaryImg, nucleiImg, flourescentImg, overlayImg};
% 
% figure
% for i = 1:numel(imgSet)
%     imshow(imgSet{i}), axis on
% end
% 
% %% Cell Boundary Segmentation Extraction
% cc = regionSegmentation(cellBoundaryImg);
% 
% cellProps = regionprops(cc, 'Area','Eccentricity','Perimeter','EquivDiameter','MajorAxisLength','MinorAxisLength','Orientation', 'Centroid', 'PixelList', 'BoundingBox'); 
% cellProps2 = regionprops(cc, 'Centroid', 'PixelList');
% cellConvexImg = regionprops(cc, 'ConvexImage');
% 
% %% Nuclei Segmentation
% % 1) use findNuclei to find centers - round to nearest integer
% % for each nuclei:
% % 2) find the nearest regions
% % 3) check if the nuclei centers lie in any regions
% % 4) update the nuclie count of that region.
% 
% % nuclei segmentation
% sensitivity = 0.80; %increase sensivitiy finds more nuclei
% [centers, radii, metrics] = findNucleiSeg(nucleiImg,sensitivity);
% 
% 
% %%
% % define parameters to assign nuclei to region
% centers = round(centers);
% cellStruct = cellProps2;
% pixelLimit = 25;
% nucleiCount = zeros(size(cellStruct,1));
% nucleiSize = zeros(size(cellStruct,1));
% 
% for k = 1:size(centers,1)
%     %disp('x')
%     %disp(k)
%     
%     % find the nearest regions
%     count = 1;
%     nearestRegions = {};
%     for j = 1:size(cellStruct,1)
%         distance = norm(centers(k,:) - cellStruct(j).Centroid);
%         if distance < pixelLimit
%             nearestRegions{count} = j;
%             count = count + 1;
%         end
%     end
%     
%     %disp('xx')
%     %disp(nearestRegions)
%     
%     % check if the nuclei centers lie in any of the nearest regions
%     nucleiAssigned = false;
%     for nNearestRegion = 1:length(nearestRegions)
%         regionInd = nearestRegions{nNearestRegion};
%         logical = ismember(centers(k,:), cellStruct(regionInd).PixelList, 'rows');
%         if logical == 1
%             %disp('xxx')
%             %disp(regionInd)
%             nucleiCount(regionInd) = nucleiCount(regionInd) + 1;
%             nucleiSize(regionInd) = nucleiCount(regionInd) + radii(k)^2;
%             nucleiAssigned = true; 
%         end
%     end 
%     
%     % if the nuclei is not assigned, assign it to the largest region
% %     if nucleiAssigned == false
% %         maxArea = -inf;
% %         for nNearestRegion = 1:length(nearestRegions)
% %             regionInd = nearestRegions{nNearestRegion};
% %             regionArea = size(cellStruct(regionInd).PixelList,1);
% %             if regionArea > maxArea
% %                 maxArea = regionArea;
% %                 maxRegionInd = regionInd;
% %             end
% %         end
% %         %disp('xxxx')
% %         %disp(maxRegionInd)
% %         nucleiAssigned = true;
% %         nucleiCount(maxRegionInd) = nucleiCount(maxRegionInd) + 1;
% %     end
% end
% 
% % display overlay of nuclei
% figure; imshow(overlayImg), axis on, hold on;
% 
% for i = 1:size(cellStruct,1)
%     txt1 = num2str(nucleiCount(i));
%     x1 = cellStruct(i).Centroid(1);
%     y1 = cellStruct(i).Centroid(2);
%     if strcmp(txt1, '0')
%         text(x1,y1,txt1,'FontSize',6,'Color','yellow')
%     elseif strcmp(txt1, '1')
%         text(x1,y1,txt1,'FontSize',8,'Color','green')
%     elseif strcmp(txt1, '2')
%         text(x1,y1,txt1,'FontSize',8,'Color','cyan')
%     else
%         text(x1,y1,txt1,'FontSize',8,'Color','magenta')
%     end
% end
% 
% % Assign nucleiCount to new assignment
% tempStruct = cellProps;
% for k = 1:length(nucleiCount)
%     tempStruct(k).nucleiCount = nucleiCount(k);
%     tempStruct(k).nucleiSize = nucleiSize(k);
% end
% cellProps = tempStruct;
% 
% noNucInd = find([cellProps.nucleiCount] == 0);
% noNucleiCellStruct = tempStruct(noNucInd); %region props without nuclei
% 
% nucInd = find([cellProps.nucleiCount] > 0);
% nucleiCellStruct = tempStruct(nucInd); %cell props containing nuclei
% 
% %% Assign cell
% 
% for k = 1 : length(cellProps)
%     if cellProps(k).nucleiCount > 0
%         cellProps(k).label = 'C';
%     else 
%         cellProps(k).label = 'NA';
%     end
% end
