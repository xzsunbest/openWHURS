function newCellProps = addDistToBDFeature(regionAssgn, cellProps)
% This function takes regionAssgn a struct, assigning region index to a label, 
% and a cellProp struct containing the properties for all region, and adds
% a new feature distance to the bile duct to each region. 

    count = 1;
    bdAssgn = {};
    for i = 1:numel(regionAssgn)
        if strcmp(regionAssgn{i}{2}, 'BD')
            bdAssgn{count} = regionAssgn{i};
            count = count + 1;
        end
    end

    for k = 1:numel(cellProps)
        minDistToBD = inf;
        for j = 1:numel(bdAssgn)
            BDind = bdAssgn{j}{1};
            BDcentroid = cellProps(BDind).Centroid;

            regionLabel = cellProps(k).label;
            if strcmp(regionLabel, 'PV') || strcmp(regionLabel, 'CV')
            % if the region is PV or CV, comput BD distance based on the
            % nearest region pixel length
                regionPixelList = cellProps(k).PixelList;
                for i = 1:length(regionPixelList)
                    distFromBD = norm(regionPixelList(i,:) - BDcentroid);
                    if distFromBD < minDistToBD
                        minDistToBD = distFromBD;
                    end
                end
            else  
                regCentroid = cellProps(k).Centroid;
                distFromBD = norm(regCentroid - BDcentroid);
                if distFromBD < minDistToBD
                    minDistToBD = distFromBD;
                end
            end
        end
        cellProps(k).distFromBD = minDistToBD;
    end
    
    newCellProps = cellProps;
end
