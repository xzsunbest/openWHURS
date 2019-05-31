function [newX, newY] = subSample(X, Y, inds, classSize)
newInds = randsample(inds, classSize);
newX = X(newInds, :);
newY = Y(newInds, :);

end

