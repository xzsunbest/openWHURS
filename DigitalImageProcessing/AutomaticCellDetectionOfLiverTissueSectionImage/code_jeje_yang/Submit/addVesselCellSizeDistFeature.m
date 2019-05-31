function newCellProps = addVesselCellSizeDistFeature(regionAssgn, cellProps)

    % Get vessel assignments
    count = 1;
    vesselAssgn = {};
    for i = 1:numel(regionAssgn)
        if strcmp(regionAssgn{i}{2}, 'CV') || strcmp(regionAssgn{i}{2}, 'PV') || strcmp(regionAssgn{i}{2}, 'BD')
            vesselAssgn{count, 1} = regionAssgn{i};
            count = count + 1;
        end
    end
    
    % Calculate the nearby cell sizes relative to its distance;
    radiusLimit = 100;
    numOfBins = 20;
    for j = 1:length(vesselAssgn)
        vesselInd = vesselAssgn{j}{1};
        vesselRadius = cellProps(vesselInd).EquivDiameter / 2; 
        vesselPixels = cellProps(vesselInd).PixelList;
        vesselCentroid = cellProps(vesselInd).Centroid;
        
        % nearbyCells in (x1, size1) distance and size of region
        nearbyCells = [];
        for k = 1:numel(cellProps)
            regionLabel = cellProps(k).label;
            regionCentroid = cellProps(k).Centroid;
            dist = norm(regionCentroid - vesselCentroid);
            
            if dist < radiusLimit + vesselRadius
                if strcmp(regionLabel, 'C') || strcmp(regionLabel, 'IC')
                    minDistToVessel = inf;
                    cellCentroid = cellProps(k).Centroid;
                    cellArea = cellProps(k).Area;
                    for i = 1:length(vesselPixels)
                        distCellToVessel = norm(cellCentroid - vesselPixels(i,:));
                        if distCellToVessel < minDistToVessel
                            minDistToVessel = distCellToVessel;
                        end
                    end
                    nearbyCells = [nearbyCells; [minDistToVessel cellArea]];
                end
            end
        end
        
        %find the histogram distribution of the cell range
        binRange = 0:radiusLimit/numOfBins:radiusLimit;
        [N,edges,bin] = histcounts(nearbyCells(:,1), binRange);
        averageCells = zeros(length(N), 2);
        %output the discretized cell distance to vessel feature
        for i = 1:length(edges)-1
            averageCells(i,1) = (edges(i) + edges(i+1))/2;
        end
        totalCellSize = zeros(length(N), 1);
        %output the average cell size of the corresponding cell distance
        for k = 1:length(bin)
            cellSize = nearbyCells(k, 2);
            if bin(k) ~= 0
                totalCellSize(bin(k)) = totalCellSize(bin(k)) + cellSize;
            end
        end
        averageCells(:,2) = totalCellSize./N';
      
        vesselAssgn{j, 2} = averageCells;  
    end
    
    %add new features to the vessels
    for i = 1:size(vesselAssgn, 1)
        ind_ = vesselAssgn{i,1}{1};
        nearbyCellSizeVsDist = vesselAssgn{i,2};
        cellProps(ind_).nearbyCellSizeVsDist = nearbyCellSizeVsDist;
    end
    
    newCellProps = cellProps;
end

                    
