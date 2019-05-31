function [newStruct] = assignNewLabel(cellProp, regionInd, Label)
    cellProp(regionInd).label = Label;
    newStruct = cellProp;
end