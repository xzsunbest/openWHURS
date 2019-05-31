function [counts, nucleiNum] = errorAnalysis(cellProps, lim)
% This function takes in a structure array containing centroids and
% nucleicount and limit of range to look into and returns the counts and
% number of nuclei in that region

    centroids = {cellProps.Centroid};
    
    regionInd = [];
    xLowerLim = lim(1);
    xUpperLim = lim(2);
    yLowerLim = lim(3);
    yUpperLim = lim(4);
    for i = 1:numel(centroids)
        if centroids{i}(1) >= xLowerLim && centroids{i}(1) <= xUpperLim ...
                && centroids{i}(2) >= yLowerLim && centroids{i}(2) <= yUpperLim
            regionInd = [regionInd i];
        end
    end

    nucleiCounts = [];
    for i = 1:numel(regionInd)
        nucleiCounts = [nucleiCounts cellProps(regionInd(i)).nucleiCount];
    end

    [counts,nucleiNum] = hist(nucleiCounts,unique(nucleiCounts));
end

