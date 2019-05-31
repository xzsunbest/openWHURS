function [class] = multiNomPredict(B, x)
%MULTINOMPREDICT Summary of this function goes here
%   Detailed explanation goes here
[m, class] = max(mnrval(B, x));
end

