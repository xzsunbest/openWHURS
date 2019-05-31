function [newX, newY] = smote(X, Y, inds, classSize, k)
%SMOTE Summary of this function goes here
%   Detailed explanation goes here
origSize = length(inds);
numSynthethic = classSize - origSize;
numAttr = size(X, 2);

classX = X(inds, :);
classY = Y(inds, :);
classVal = classY(1);

newX = zeros(numSynthethic, numAttr);
newY = classVal * ones(numSynthethic, 1);

IDX = knnsearch(X, classX, 'K', k);

cur = 1;
while numSynthethic > 0
    classInd = 1 + mod(cur - 1, origSize); % Index of class example
    nnInds = IDX(classInd, :); % vector of indexes of nearest neighbors
    nn = nnInds(randi(k)); % nearest neighbor to use
    
    sample = classX(classInd, :);
    neighbor = X(nn, :);
    
    dif = neighbor - sample;
    gap = rand([1 numAttr]);
    
    newX(cur, :) = sample + gap .* dif;
    
    numSynthethic = numSynthethic - 1;
end

newX = [newX; classX];
newY = [newY; classY];

end

