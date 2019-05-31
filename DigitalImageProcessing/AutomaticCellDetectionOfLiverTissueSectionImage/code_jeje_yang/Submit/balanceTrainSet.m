function [newX, newY] = balanceTrainSet(X, Y)
%BALANCETRAINSET Summary of this function goes here
%   Detailed explanation goes here

labels = unique(Y)';
labelIndexes = containers.Map('KeyType',  'uint32', 'ValueType', 'any');
for l = labels
    labelIndexes(l) = find(Y == l);
end

classSize = round(length(Y) / length(labels));

newX = zeros(classSize * length(labels), size(X, 2));
newY = zeros(classSize * length(labels), size(Y, 2));

cur = 1;
for l = labels
    inds = labelIndexes(l);
    if length(inds) < classSize
        [modifiedX, modifiedY] = smote(X, Y, inds, classSize, 4);
    elseif length(inds) > classSize
        [modifiedX, modifiedY] = subSample(X, Y, inds, classSize);
    else
        modifiedX = X(inds);
        modifiedY = Y(inds);
    end
    newX(cur:cur + classSize - 1, :) = modifiedX;
    newY(cur:cur + classSize - 1, :) = modifiedY;
    cur = cur + classSize;
end

end

