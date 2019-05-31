 %% Segmentation of Cell Tissue
close all; clc; clearvars;

%% Read in set of images
filename = 'DZ6081a_02A08';

cellBoundaryImg = im2double(imread(strcat(filename, ' Phalloidin Cy5 10x.tif')));
nucleiImg = im2double(imread(strcat(filename, ' DAPI 10x.tif')));
flourescentImg = im2double(imread(strcat(filename, ' DsRed 10x.tif')));
overlayImg = im2double(imread(['merge', ' ', filename, ' 10x.tif']));
imgSet = {cellBoundaryImg, nucleiImg, flourescentImg, overlayImg};

figure
for i = 1:numel(imgSet) 
    imshow(imgSet{i}), axis on
end

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


%%
% define parameters to assign nuclei to region
centers = round(centers);
cellStruct = cellProps2;
pixelLimit = 25;
nucleiCount = zeros(size(cellStruct,1));
nucleiSize = zeros(size(cellStruct,1));

for k = 1:size(centers,1)
    %disp('x')
    %disp(k)
    
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
    
    %disp('xx')
    %disp(nearestRegions)
    
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
    
    % if the nuclei is not assigned, assign it to the largest region
%     if nucleiAssigned == false
%         maxArea = -inf;
%         for nNearestRegion = 1:length(nearestRegions)
%             regionInd = nearestRegions{nNearestRegion};
%             regionArea = size(cellStruct(regionInd).PixelList,1);
%             if regionArea > maxArea
%                 maxArea = regionArea;
%                 maxRegionInd = regionInd;
%             end
%         end
%         %disp('xxxx')
%         %disp(maxRegionInd)
%         nucleiAssigned = true;
%         nucleiCount(maxRegionInd) = nucleiCount(maxRegionInd) + 1;
%     end
end

% display overlay of nuclei
figure; imshow(overlayImg), axis on, hold on;

for i = 1:size(cellStruct,1)
    txt1 = num2str(nucleiCount(i));
    x1 = cellStruct(i).Centroid(1);
    y1 = cellStruct(i).Centroid(2);
    if strcmp(txt1, '0')
        %text(x1,y1,txt1,'FontSize',8,'Color','yellow')
    elseif strcmp(txt1, '1')
        text(x1,y1,txt1,'FontSize',10,'Color','green')
    elseif strcmp(txt1, '2')
        text(x1,y1,txt1,'FontSize',10,'Color','cyan')
    else
        text(x1,y1,txt1,'FontSize',10,'Color','magenta')
    end
end

% Assign nucleiCount to new assignment
tempStruct = cellProps;
for k = 1:length(nucleiCount)
    tempStruct(k).nucleiCount = nucleiCount(k);
    tempStruct(k).nucleiSize = nucleiSize(k);
end
cellProps = tempStruct;

noNucInd = find([cellProps.nucleiCount] == 0);
noNucleiCellStruct = tempStruct(noNucInd); %region props without nuclei

nucInd = find([cellProps.nucleiCount] > 0);
nucleiCellStruct = tempStruct(nucInd); %cell props containing nuclei

%% Error analysis
lim = [850 1220 480 850];
[count, nucCount] = errorAnalysis(cellProps, lim)

%% k-means clustering
% cluster the three different types of empty regions without nuclei, including central veins,
% portal veins, and other empty spaces (intracellular)
K1 = 3;
showRegionNumber = true;

noNucIndClusterAssgn = num2cell([noNucInd', zeros(size(noNucInd'))]);
nucIndClusterAssgn = num2cell([nucInd', zeros(size(nucInd'))]);  
nonNucClusters = kmeans(cell2mat(struct2cell(rmfield(noNucleiCellStruct, {'Centroid', 'PixelList', 'BoundingBox'})))', K1, 'Start', 'sample');

for j = 1 : K1
    inds = find(nonNucClusters == j);
    for i = 1:length(inds)
        assgn_ = strcat('noNuc', num2str(j));
        noNucIndClusterAssgn{inds(i),2} = assgn_;
    end
   
    figure, imshow(overlayImg), title(sprintf('NonNuclei Cluster %d, %d matches', j, length(inds))), axis on, hold on;
    for ind = inds'
        x1 = noNucleiCellStruct(ind).Centroid(1); 
        y1 = noNucleiCellStruct(ind).Centroid(2);
        regionInd = num2str(noNucIndClusterAssgn{ind,1});
        if showRegionNumber
            text(x1,y1,regionInd,'FontSize',8,'Color','green')
        else
            plot(x1,y1,'gx')
        end
    end
end

% cluster three different types of cells, including hepatocyptes (big round nuclei),
% endothethial cells (small flat/thin nuclei), and bile duct epithelial
% cells ( small, round nuclei)
K2 = 3;
NucClusters = kmeans(cell2mat(struct2cell(rmfield(nucleiCellStruct, {'Centroid', 'PixelList', 'BoundingBox'})))', K2, 'Start', 'sample');

for j = 1 : K2
    inds = find(NucClusters == j);
    for i = 1:length(inds)
        assgn_ = strcat('Nuc', num2str(j));
        nucIndClusterAssgn{inds(i),2} = assgn_;
    end

    figure, imshow(overlayImg), title(sprintf('Nuclei Cluster %d, %d matches', j, length(inds))), axis on, hold on;
    for ind = inds'
        x1 = nucleiCellStruct(ind).Centroid(1); 
        y1 = nucleiCellStruct(ind).Centroid(2);
        regionInd = num2str(nucIndClusterAssgn{ind,1});
        if showRegionNumber
            text(x1,y1,regionInd,'FontSize',8,'Color','green')
        else
            plot(x1,y1,'gx')
        end
    end
end

%% Assign Labels - Manually Done using clustering and cross referencing 
% with expert labeling

% adjust based on clustering results:
labelAssgn = {'Nuc1', 'C';
               'Nuc2', 'C';
               'Nuc3', 'C';
               'noNuc1', 'IC';
               'noNuc2', 'IC';
               'noNuc3', 'IC';};
ClusterAssgn = [noNucIndClusterAssgn; nucIndClusterAssgn];

tempStruct = cellProps;
for k = 1:size(ClusterAssgn,1)
    ind = ClusterAssgn{k,1};
    assgn = ClusterAssgn{k,2};
    label = labelAssgn{strcmp(labelAssgn,assgn),2};
    tempStruct(ind).label = label;
end
cellProps = tempStruct;

% manually assign specific labels
regionAssgn = {{1472, 'CV'}, {942, 'PV'}, {2030, 'CV'}, {931, 'BD'}};
%     {1090, 'PV'}, {19, 'PV'}, ...
%                {116, 'PV'}, {1780, 'CV'}, {1412, 'CV'}, ...
%                {1180, 'BD'}}; 

for k = 1:numel(regionAssgn)
    regionInd = regionAssgn{k}{1};
    Label = regionAssgn{k}{2};
    cellProps(regionInd).label = Label;
end

% display overlay of region labels
figure; imshow(overlayImg), axis on, hold on;
cellStruct = cellProps;
for i = 1:size(cellStruct,1)
    txt1 = cellStruct(i).label;
    x1 = cellStruct(i).Centroid(1);
    y1 = cellStruct(i).Centroid(2);
    if strcmp(txt1, 'CV') || strcmp(txt1, 'PV')
        text(x1,y1,txt1,'FontSize',6,'Color','green')
    elseif strcmp(txt1, 'BD')
        text(x1,y1,txt1,'FontSize',8,'Color','cyan')
    elseif strcmp(txt1, 'C')
        text(x1,y1,txt1,'FontSize',6,'Color','magenta')
    else
        text(x1,y1,txt1,'FontSize',6,'Color','yellow')
    end
end

% Add feature: distance to bileduct
cellProps = addDistToBDFeature(regionAssgn, cellProps);

%%
% Add feature: cell size relative to distance from a vessel (CV or PV)
cellProps = addVesselCellSizeDistFeature(regionAssgn, cellProps);

%% sanity check
disp('PV')
find(strcmp({cellProps.label}, 'PV'))
cellInd_ = find(strcmp({cellProps.label}, 'PV'));
if not(isempty(cellInd_ ))
    cellProps(cellInd_(1))
end

disp('BD')
cellInd_ = find(strcmp({cellProps.label}, 'BD'))
randInd = max(1, round(rand(1)*length(cellInd_)))
if not(isempty(cellInd_ ))
    cellProps(cellInd_(randInd))
end

disp('CV')
find(strcmp({cellProps.label}, 'CV'))
cellInd_ = find(strcmp({cellProps.label}, 'CV'));
if not(isempty(cellInd_ ))
    cellProps(cellInd_(1))
end

disp('Cell')
cellInd_ = find(strcmp({cellProps.label}, 'C'));
randInd = round(rand(1)*length(cellInd_))
cellProps(cellInd_(randInd))

disp('IntraCell')
cellInd_ = find(strcmp({cellProps.label}, 'IC'));
randInd = round(rand(1)*length(cellInd_))
cellProps(cellInd_(randInd))

%% save appropriate cell features vectors
savedFilename = strcat('newFeatures3\',filename,'.mat');
save(savedFilename, 'cellProps', 'cellConvexImg')